#include <memory.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "../../zlib/zlib.h"
#include "../img_data.h"
#include "inner_include.h"

#pragma pack(push)
#pragma pack(1)
enum class PNGColorFormat : int
{
	GrayScale = 0,
	Invalid = 1,
	TrueColor = 2,
	IndexedColor = 3,
};

enum class PNGFilterMethod : uint8_t
{
	None = 0,
	Sub = 1,
	Up = 2,
	Average = 3,
	Paeth = 4,
};

struct PNGFileHead
{
	uint32_t MagicPNG;	//0x89+'PNG'
	uint32_t MagicCRLF;	//CRLF

	bool IsPNG()
	{
		const unsigned int kPNGMagic = 0x474E5089;
		const unsigned int kCRLFMagic = 0x0A1A0A0D;
		return MagicPNG == kPNGMagic && MagicCRLF == kCRLFMagic;
	}
};

struct PNGChunkHead
{
	uint32_t Length;
	uint32_t Type;
};

struct PNGChunkIHDR
{
	uint32_t Width;
	uint32_t Height;
	uint8_t  EachColorBits;
	uint8_t  Format;
	uint8_t  Compression;
	uint8_t  Filter;
	uint8_t  Interlace;

	PNGColorFormat GetColorFormat()const
	{
		return (PNGColorFormat)(Format & 0x3);
	}

	bool HaveAlphaChannel() const
	{
		return (Format & 0x4) > 0;
	}
};
#pragma pack(pop)
constexpr const int kSizePNGFileHead = sizeof(PNGFileHead);
constexpr const int kSizePNGChunkHead = sizeof(PNGChunkHead);
constexpr const int kSizePNGChunkCRC = sizeof(uint32_t);

uint32_t swap_endian(uint32_t u)
{
	return ((u & 0xFF) << 24) | ((u & 0xFF00) << 8) | ((u & 0xFF0000) >> 8) | ((u & 0xFF000000) >> 24);
}

#define FourCC(x) constexpr const uint32_t CHUNK_##x = MakeFourCC(#x);
FourCC(IHDR);
FourCC(PLTE);
FourCC(IDAT);
FourCC(IEND);
FourCC(tRNS);
#undef FourCC

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c)
{
	auto p = a + b - c;
	auto pa = abs(p - a);
	auto pb = abs(p - b);
	auto pc = abs(p - c);

	if (pa <= pb && pa <= pc)
		return a;
	if (pb <= pc)
		return b;
	return c;
}

bool is_png(uint8_t* file_data)
{
	auto fileHead = (PNGFileHead*)file_data;
	if (fileHead->IsPNG())
	{
		auto dataPtr = file_data + kSizePNGFileHead;
		auto chunkHead = (PNGChunkHead*)dataPtr;
		if (chunkHead->Type == CHUNK_IHDR)
		{
			auto  IHDR = (PNGChunkIHDR*)(dataPtr + kSizePNGChunkHead);
			if (IHDR->Compression == 0 && IHDR->Filter == 0)
			{
				if (IHDR->GetColorFormat() == PNGColorFormat::TrueColor && IHDR->EachColorBits == 8)
				{
					return true;
				}
				else if (IHDR->GetColorFormat() == PNGColorFormat::IndexedColor)
				{
					if (IHDR->Interlace != 0)
					{
						return IHDR->EachColorBits == 8;
					}
					else
					{
						return IHDR->EachColorBits == 1
							|| IHDR->EachColorBits == 2
							|| IHDR->EachColorBits == 4
							|| IHDR->EachColorBits == 8;
					}
				}
			}
		}
	}

	return false;
}

void expand_bits_to_bytes(std::vector<uint8_t>& decodedData, uint32_t pitch, uint32_t height, uint32_t bitCount)
{
	constexpr uint8_t bitMask1[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
	constexpr uint8_t bitMask2[] = { 0xC0, 0x30, 0x0C, 0x03 };
	constexpr uint8_t bitMask4[] = { 0xF0, 0x0F };

	constexpr uint8_t shift1[] = { 7, 6, 5, 4, 3, 2, 1, 0 };
	constexpr uint8_t shift2[] = { 6, 4, 2, 0 };
	constexpr uint8_t shift4[] = { 4, 0 };

	uint32_t maskCount = 0;
	const uint8_t* masks = nullptr;
	const uint8_t* shifts = nullptr;;

	if (bitCount == 1)
	{
		maskCount = 8;
		masks = bitMask1;
		shifts = shift1;
	}
	else if (bitCount == 2)
	{
		maskCount = 4;
		masks = bitMask2;
		shifts = shift2;
	}
	else if (bitCount == 4)
	{
		maskCount = 2;
		masks = bitMask4;
		shifts = shift4;
	}

	auto srcWidth = pitch - 1;
	auto dstWidth = srcWidth * 8 / bitCount;
	auto dstPitch = dstWidth + 1;

	std::vector<uint8_t> expandData(dstPitch* height);

	for (uint32_t h = 0; h < height; h++)
	{
		auto lineData = &(decodedData[h * pitch]);
		auto offset = dstPitch * h;
		expandData[offset] = *lineData++;
		auto dstIndex = &(expandData[offset + 1]);
		for (uint32_t w = 0; w < srcWidth; w++)
		{
			for (uint32_t index = 0; index < maskCount; index++)
			{
				*dstIndex++ = (lineData[w] & masks[index]) >> (shifts[index]);
			}
		}
	}
	decodedData.swap(expandData);
}

bool reverse_filtering(uint8_t* data, uint32_t width, uint32_t height, uint32_t colorSpace)
{
	auto lineSize = width * colorSpace;
	auto linePitch = 1 + lineSize;

	for (uint32_t row = 0; row < height; row++)
	{
		auto lineData = data + linePitch * row + 1;
		auto filteringMethod = (PNGFilterMethod)data[linePitch * row];
		if (filteringMethod == PNGFilterMethod::None)
		{
		}
		else if (filteringMethod == PNGFilterMethod::Sub)
		{
			for (uint32_t j = 1; j < width; j++)
			{
				auto reconA = lineData + (j - 1) * colorSpace;
				auto srcColor = lineData + j * colorSpace;
				for (uint32_t c = 0; c < colorSpace; c++)
				{
					srcColor[c] = (srcColor[c] + reconA[c]) & 0xFF;
				}
			}
		}
		else if (filteringMethod == PNGFilterMethod::Up)
		{
			if (row > 0)
			{
				auto reconB = data + linePitch * (row - 1) + 1;
				for (uint32_t j = 0; j < lineSize; j++)
				{
					lineData[j] = (lineData[j] + reconB[j]) & 0xFF;
				}
			}
		}
		else if (filteringMethod == PNGFilterMethod::Average)
		{
			if (row == 0)
			{
				for (uint32_t j = 1; j < width; j++)
				{
					auto reconA = lineData + (j - 1) * colorSpace;
					auto srcColor = lineData + j * colorSpace;
					for (uint32_t c = 0; c < colorSpace; c++)
					{
						srcColor[c] = (srcColor[c] + reconA[c] / 2) & 0xFF;
					}
				}
			}
			else
			{
				auto reconBs = data + linePitch * (row - 1) + 1;
				for (uint32_t c = 0; c < colorSpace; c++)
				{
					lineData[c] = (lineData[c] + reconBs[c] / 2) & 0xFF;
				}

				for (uint32_t j = 1; j < width; j++)
				{
					auto reconA = lineData + (j - 1) * colorSpace;
					auto reconB = reconBs + j * colorSpace;
					auto srcColor = lineData + j * colorSpace;
					for (uint32_t c = 0; c < colorSpace; c++)
					{
						srcColor[c] = (srcColor[c] + (reconA[c] + reconB[c]) / 2) & 0xFF;
					}
				}
			}
		}
		else if (filteringMethod == PNGFilterMethod::Paeth)
		{
			if (row == 0)
			{
				for (uint32_t j = 1; j < width; j++)
				{
					auto reconA = lineData + (j - 1) * colorSpace;
					auto srcColor = lineData + j * colorSpace;
					for (uint32_t c = 0; c < colorSpace; c++)
					{
						//color[c] = (color[c] + PaethPredictor(reconA[c], 0, 0)) & 0xFF;
						srcColor[c] = (srcColor[c] + reconA[c]) & 0xFF;
					}
				}
			}
			else
			{
				auto reconBs = data + linePitch * (row - 1) + 1;
				for (uint32_t c = 0; c < colorSpace; c++)
				{
					//lineData[c] = (lineData[c] + PaethPredictor(0, reconBs[c], 0)) & 0xFF;
					lineData[c] = (lineData[c] + reconBs[c]) & 0xFF;
				}

				for (uint32_t j = 1; j < width; j++)
				{
					auto reconA = lineData + (j - 1) * colorSpace;
					auto reconB = reconBs + j * colorSpace;
					auto reconC = reconBs + (j - 1) * colorSpace;
					auto srcColor = lineData + j * colorSpace;
					for (uint32_t c = 0; c < colorSpace; c++)
					{
						srcColor[c] = (srcColor[c] + paeth_predictor(reconA[c], reconB[c], reconC[c])) & 0xFF;
					}
				}
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

img_data parsing_indexed_color(PNGChunkIHDR* IHDR, std::vector<uint8_t>& iDataBuffer,
	ColorTable24& colorTable, AlphaTable& alphaTable);

img_data parsing_truecolor(PNGChunkIHDR* IHDR, std::vector<uint8_t>& iDataBuffer);



img_data read_png(uint8_t* file_data)
{
	auto dataPtr = file_data + kSizePNGFileHead;
	auto chunkHead = (PNGChunkHead*)dataPtr;

	dataPtr += kSizePNGChunkHead;
	auto IHDR = (PNGChunkIHDR*)dataPtr;

	// find color table data
	// find scanline datas
	ColorTable24 colorTable;
	AlphaTable alphaTable;
	alphaTable.def_value = 255;

	std::vector<uint8_t> iDataBuffer;
	while (chunkHead->Type != CHUNK_IEND)
	{
		dataPtr += swap_endian(chunkHead->Length) + kSizePNGChunkCRC;
		chunkHead = (PNGChunkHead*)dataPtr;
		dataPtr += kSizePNGChunkHead;
		// color table
		if (chunkHead->Type == CHUNK_PLTE)
		{
			colorTable.table = (BGR24*)dataPtr;
			colorTable.count = swap_endian(chunkHead->Length) / 3;
		}
		// compressed image data
		else if (chunkHead->Type == CHUNK_IDAT)
		{
			auto dataLength = swap_endian(chunkHead->Length);
			auto bufferSize = iDataBuffer.size();
			iDataBuffer.resize(bufferSize + dataLength);
			memcpy(&(iDataBuffer[bufferSize]), dataPtr, dataLength);
		}
		else if (chunkHead->Type == CHUNK_tRNS)
		{
			alphaTable.table = dataPtr;
			alphaTable.count = swap_endian(chunkHead->Length);
		}
	}

	//have no imaged data
	if (iDataBuffer.size() == 0)
	{
		return img_data();
	}

	if (IHDR->GetColorFormat() == PNGColorFormat::IndexedColor)
	{
		if (colorTable)
			return parsing_indexed_color(IHDR, iDataBuffer, colorTable, alphaTable);
		else
			return img_data();
	}
	else
	{
		return parsing_truecolor(IHDR, iDataBuffer);
	}
}

struct InterlacedBlock
{
	uint32_t width;
	uint32_t height;
	uint8_t* buffer;
	uint8_t* get_pixel(uint32_t w, uint32_t h, uint32_t colorSpace)
	{
		return buffer + 1 + h * (width * colorSpace + 1) + w * colorSpace;
	}
};

const int kInterlacedPass = 7;
constexpr uint32_t kRowStarts[kInterlacedPass] = { 0, 0, 4, 0, 2, 0, 1 };
constexpr uint32_t kColStarts[kInterlacedPass] = { 0, 4, 0, 2, 0, 1, 0 };
constexpr uint32_t kRowStrides[kInterlacedPass] = { 8, 8, 8, 4, 4, 2, 2 };
constexpr uint32_t kColStrides[kInterlacedPass] = { 8, 8, 4, 4, 2, 2, 1 };
constexpr uint32_t kBlockWidths[kInterlacedPass] = { 8, 4, 4, 2, 2, 1, 1 };
constexpr uint32_t kBlockHeights[kInterlacedPass] = { 8, 8, 4, 4, 2, 2, 1 };

img_data parsing_indexed_color(PNGChunkIHDR* IHDR, std::vector<uint8_t>& iDataBuffer,
	ColorTable24& colorTable, AlphaTable& alphaTable)
{
	typedef void(*SetColor)(uint8_t* dstColor, uint32_t index, ColorTable24& colorTable, AlphaTable& alphaTable);
	SetColor setColor;

	if (alphaTable)
	{
		setColor = [](uint8_t* dstColor, uint32_t index, ColorTable24& colorTable, AlphaTable& alphaTable)
		{
			memcpy(dstColor, colorTable[index], 3);
			dstColor[3] = *(alphaTable[index]);
		};
	}
	else
	{
		setColor = [](uint8_t* dstColor, uint32_t index, ColorTable24& colorTable, AlphaTable& alphaTable)
		{
			memcpy(dstColor, colorTable[index], 3);
		};
	}

	auto width = swap_endian(IHDR->Width);
	auto height = swap_endian(IHDR->Height);
	unsigned long encodedDataLength = (unsigned long)iDataBuffer.size();
	unsigned long decodedDataLength = (width + 1) * height;
	std::vector<uint8_t> decodedData(decodedDataLength);

	auto result = uncompress(&(decodedData[0]), &decodedDataLength, &(iDataBuffer[0]), encodedDataLength);
	while (Z_BUF_ERROR == result)
	{
		decodedDataLength += decodedDataLength / 2;
		decodedData.resize(decodedDataLength);
		result = uncompress(&(decodedData[0]), &decodedDataLength, &(iDataBuffer[0]), encodedDataLength);
	}
	if (Z_OK != result)
	{
		return img_data();
	}

	if (IHDR->Interlace == 0)
	{
		if (IHDR->EachColorBits != 8)
		{
			//swap color table components.
			for (uint32_t i = 0; i < colorTable.count; i++)
			{
				auto color = colorTable[i];
				auto b = color->b;
				color->b = color->r;
				color->r = b;
			}

			//expand bits to byte
			expand_bits_to_bytes(decodedData, decodedDataLength / height, height, IHDR->EachColorBits);
			decodedDataLength = decodedData.size();
		}

		if (!reverse_filtering(&(decodedData[0]), width, height, 1))
		{
			return img_data();
		}

		auto srcPitch = decodedDataLength / height;
		img_data image;
		if (alphaTable)
		{
			set_img_data_bgra32(image, width, height);
		}
		else
		{
			set_img_data_bgr24(image, width, height);
		}

		for (uint32_t row = 0; row < image.height; row++)
		{
			auto srcPtr = &(decodedData[srcPitch * row + 1]);
			for (uint32_t col = 0; col < image.width; col++)
			{
				auto& index = srcPtr[col];
				auto dstColor = image.get_pixel(col, row);
				setColor(dstColor, index, colorTable, alphaTable);
			}
		}

		return image;
	}
	else
	{
		//interlaced
		InterlacedBlock colorPlane[kInterlacedPass];

		auto dataBuffer = &(decodedData[0]);
		for (int pass = 0; pass < kInterlacedPass; pass++)
		{
			auto& plane = colorPlane[pass];
			plane.width = (width + kColStrides[pass] - 1 - kColStarts[pass]) / kColStrides[pass];
			plane.height = (height + kRowStrides[pass] - 1 - kRowStarts[pass]) / kRowStrides[pass];
			plane.buffer = dataBuffer;
			dataBuffer += (plane.width + 1) * plane.height;

			if (!reverse_filtering(plane.buffer, plane.width, plane.height, 1))
			{
				return img_data();
			}
		}

		img_data image;
		if (alphaTable)
		{
			set_img_data_bgra32(image, width, height);
		}
		else
		{
			set_img_data_bgr24(image, width, height);
		}

		for (int pass = 0; pass < kInterlacedPass; pass++)
		{
			auto& plane = colorPlane[pass];
			auto index = plane.buffer + 1;
			for (uint32_t h = 0; h < plane.height; h++)
			{
				auto row = kRowStarts[pass] + h * kRowStrides[pass];
				for (uint32_t w = 0; w < plane.width; w++)
				{
					auto col = kColStarts[pass] + w * kColStrides[pass];
					auto colorIndex = *(plane.get_pixel(w, h, 1));
					auto dstColor = image.get_pixel(col, row);
					setColor(dstColor, colorIndex, colorTable, alphaTable);
				}
			}
		}
		return image;
	}
}

img_data parsing_truecolor(PNGChunkIHDR* IHDR, std::vector<uint8_t>& iDataBuffer)
{
	auto width = swap_endian(IHDR->Width);
	auto height = swap_endian(IHDR->Height);
	unsigned int colorSpace = (IHDR->HaveAlphaChannel() ? 4 : 3);
	unsigned long encodedDataLength = (unsigned long)iDataBuffer.size();
	unsigned long decodedDataLength = (width + 1) * colorSpace * height;
	std::vector<uint8_t> decodedData(decodedDataLength);

	auto result = uncompress(&(decodedData[0]), &decodedDataLength, &(iDataBuffer[0]), encodedDataLength);
	while (Z_BUF_ERROR == result)
	{
		decodedDataLength += decodedDataLength / 2;
		decodedData.resize(decodedDataLength);
		result = uncompress(&(decodedData[0]), &decodedDataLength, &(iDataBuffer[0]), encodedDataLength);
	}
	if (Z_OK != result)
	{
		return img_data();
	}

	if (IHDR->Interlace == 0)
	{
		if (!reverse_filtering(&(decodedData[0]), width, height, colorSpace))
		{
			return img_data();
		}

		auto srcPitch = decodedDataLength / height;
		img_data image;
		if (IHDR->HaveAlphaChannel())
		{
			set_img_data_bgra32(image, width, height);
		}
		else
		{
			set_img_data_bgr24(image, width, height);
		}

		for (uint32_t row = 0; row < image.height; row++)
		{
			auto srcPtr = &(decodedData[srcPitch * row + 1]);
			auto dstPtr = image.get_row(row);
			memcpy(dstPtr, srcPtr, image.line_pitch);
		}
		return image;
	}
	else
	{
		InterlacedBlock colorPlane[kInterlacedPass];

		auto dataBuffer = &(decodedData[0]);
		for (int pass = 0; pass < kInterlacedPass; pass++)
		{
			auto& plane = colorPlane[pass];
			plane.width = (width + kColStrides[pass] - 1 - kColStarts[pass]) / kColStrides[pass];
			plane.height = (height + kRowStrides[pass] - 1 - kRowStarts[pass]) / kRowStrides[pass];
			plane.buffer = dataBuffer;
			dataBuffer += (plane.width * colorSpace + 1) * plane.height;

			reverse_filtering(plane.buffer, plane.width, plane.height, colorSpace);
		}

		img_data image;
		if (IHDR->HaveAlphaChannel())
		{
			set_img_data_bgra32(image, width, height);
		}
		else
		{
			set_img_data_bgr24(image, width, height);
		}
		for (int pass = 0; pass < kInterlacedPass; pass++)
		{
			auto& plane = colorPlane[pass];
			auto index = plane.buffer + 1;
			for (uint32_t h = 0; h < plane.height; h++)
			{
				auto row = kRowStarts[pass] + h * kRowStrides[pass];
				for (uint32_t w = 0; w < plane.width; w++)
				{
					auto col = kColStarts[pass] + w * kColStrides[pass];
					auto srcColor = plane.get_pixel(w, h, image.pixel_stride);
					auto dstColor = image.get_pixel(col, row);
					memcpy(dstColor, srcColor, image.pixel_stride);
				}
			}
		}
		return image;
	}
}