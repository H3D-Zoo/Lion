#include <vector>
#include "../img_data.h"
#include "inner_include.h"

#pragma pack(push)
#pragma pack(1)
struct BMPFileHead
{
	uint16_t	MagicBM;	//0x4D42
	uint32_t	FileSize;	// whole file size.
	uint16_t	Reserved1;
	uint16_t	Reserved2;
	uint32_t	DataOffset;	// in bytes. rgbÊý¾ÝÆ«ÒÆÖµ¡£

	bool IsBMP() const
	{
		constexpr const uint16_t kBMPMagic = 0x4D42; // 'B' 'M'
		return MagicBM == kBMPMagic;
	}
};

struct BMPInfoHead
{
	uint32_t	InfoSize;	// =sizeof(BitmapInfoHead) =sizeof(this)
	int32_t		Width;
	int32_t		Height;
	uint16_t	Plane;		// always = 1, compare to other format like yuv, i420, etc.
	uint16_t	DataBits;	// normally use 24or32, 1, 4 is special used for compressed.
	uint32_t	Format;		// must be 0 for uncompressed here.
	uint32_t	DataSize;	// raw data size.
	int32_t		PPMX;
	int32_t		PPMY;
	uint32_t	ClrUsed;	//palette used
	uint32_t	ClrImportant;

	bool IsIndexedColor() const { return ClrUsed != 0; }

	unsigned int GetColorTableCount() const { return ClrUsed; }

	bool IsCompressed() const { return Format != 0; }
};
#pragma pack(pop)


constexpr const int kSizeBMPFileHead = sizeof(BMPFileHead);
constexpr const int kSizeBMPInfoHead = sizeof(BMPInfoHead);

bool is_bmp(uint8_t* file_data)
{
	auto fileHead = (BMPFileHead*)file_data;
	if (fileHead->IsBMP())
	{
		auto infoHead = (BMPInfoHead*)(file_data + kSizeBMPFileHead);
		if (!infoHead->IsCompressed() && infoHead->Plane == 1)
		{
			if (infoHead->IsIndexedColor())
			{
				return infoHead->DataBits == 1 || infoHead->DataBits == 2 || infoHead->DataBits == 4 || infoHead->DataBits == 8;
			}
			else
			{
				return infoHead->DataBits == 24 || infoHead->DataBits == 32;
			}
		}
	}
	return false;
}

void expand_bit_to_bytes(uint8_t* bits, std::vector<uint8_t>& bytes, uint32_t srcPitch, uint32_t width, uint32_t height, uint32_t bitCount)
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

	auto srcWidth = width * bitCount / 8;
	bytes.resize(width * height);

	for (uint32_t h = 0; h < height; h++)
	{
		auto lineData = &(bits[h * srcPitch]);
		auto dstIndex = &(bytes[width * h]);
		for (uint32_t w = 0; w < srcWidth; w++)
		{
			for (uint32_t index = 0; index < maskCount; index++)
			{
				*dstIndex++ = (lineData[w] & masks[index]) >> (shifts[index]);
			}
		}
	}
}

img_data read_bmp(uint8_t* file_data)
{
	auto fileHead = (BMPFileHead*)file_data;
	auto infoHead = (BMPInfoHead*)(file_data + kSizeBMPFileHead);

	//Indexed Color
	if (infoHead->IsIndexedColor())
	{
		img_data image;
		set_img_data_bgr24(image, infoHead->Width, infoHead->Height);

		// locate color table.
		ColorTable32 colorTable;
		colorTable.count = infoHead->GetColorTableCount();
		colorTable.table = (BGRA32*)(file_data + sizeof(BMPFileHead) + sizeof(BMPInfoHead));

		// fill scanline datas.
		auto dataPtr = (file_data + fileHead->DataOffset);
		auto srcPitch = line_size4(infoHead->Width, infoHead->DataBits);
		auto indexBits = infoHead->DataBits;
		std::vector<uint8_t> colorBytes;
		if (indexBits != 8)
		{
			expand_bit_to_bytes(dataPtr, colorBytes, srcPitch, image.width, image.height, indexBits);
			dataPtr = &(colorBytes[0]);
		}

		for (uint32_t row = 0; row < image.height; row++)
		{
			for (uint32_t col = 0; col < image.width; col++)
			{
				auto srcPtr = dataPtr + row * image.width + col;
				auto dstPtr = image.get_pixel(col, image.height - row - 1);
				memcpy(dstPtr, colorTable[*srcPtr], 3);
			}
		}
		return image;
	}
	else
	{
		img_data image;
		if (infoHead->DataBits == 32)
		{
			set_img_data_bgra32(image, infoHead->Width, infoHead->Height);
		}
		else
		{
			set_img_data_bgr24(image, infoHead->Width, infoHead->Height);
		}

		auto dataPtr = (file_data + fileHead->DataOffset);
		auto srcPitch = line_size4(infoHead->Width, infoHead->DataBits);
		for (uint32_t row = 0; row < image.height; row++)
		{
			auto srcPtr = dataPtr + row * srcPitch;
			auto dstPtr = image.get_row(image.height - row - 1);
			memcpy(dstPtr, srcPtr, image.line_pitch);
		}
		return image;
	}
}