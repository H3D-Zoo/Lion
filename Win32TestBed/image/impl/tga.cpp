#include <vector>
#include "inner_include.h"
#include "../img_data.h"

enum class TGAColorType : int
{
	NoImage = 0,		//colormap:no 
	IndexedColor = 1,	//colormap:yes
	TrueColor = 2,		//colormap:no 
	Grayscale = 3,		//colormap:no 
};
#pragma pack(push)
#pragma pack(1)
struct TGAFileHead
{
	uint8_t		IDLength;        /* 00h  Size of Image ID field */
	uint8_t		ColorMapType;    /* 01h  Color map type */
	uint8_t		ImageType;       /* 02h  Image type code */
	uint16_t	CMapStart;       /* 03h  Color map origin */
	uint16_t	CMapLength;      /* 05h  Color map length */
	uint8_t		CMapDepth;       /* 07h  Depth of color map entries */
	uint16_t	XOffset;         /* 08h  X origin of image */
	uint16_t	YOffset;         /* 0Ah  Y origin of image */
	uint16_t	Width;           /* 0Ch  Width of image */
	uint16_t	Height;          /* 0Eh  Height of image */
	uint8_t		PixelDepth;      /* 10h  Image pixel size */
	uint8_t		ImageDescriptor; /* 11h  Image descriptor byte */

	TGAColorType GetColorType() const
	{
		return (TGAColorType)(ImageType & 0x7);
	}

	bool IsEncoded()
	{
		return (ImageType & 0x8) > 0;
	}

	bool HaveColorTable()
	{
		return ColorMapType == 1 && GetColorTableCount() > 0;
	}

	bool NoColorTable()
	{
		return ColorMapType == 0 && GetColorTableCount() == 0;
	}

	unsigned int GetColorTableCount()
	{
		return CMapLength > CMapStart ? CMapLength - CMapStart : 0;
	}
};
#pragma pack(pop)

constexpr const int kSizeTGAFileHead = sizeof(TGAFileHead);

bool is_tga(uint8_t* file_data)
{
	auto  fileHead = (TGAFileHead*)file_data;
	if (fileHead->XOffset == 0 && fileHead->YOffset == 0 && !fileHead->IsEncoded())
	{
		if (fileHead->GetColorType() == TGAColorType::TrueColor)
		{
			return fileHead->NoColorTable()
				&& (fileHead->PixelDepth == 24 || fileHead->PixelDepth == 32);
		}
		else if (fileHead->GetColorType() == TGAColorType::IndexedColor)
		{
			return fileHead->HaveColorTable()
				&& fileHead->CMapDepth == 24
				&& (fileHead->PixelDepth == 8);
		}
		else if (fileHead->GetColorType() == TGAColorType::Grayscale)
		{
			return fileHead->PixelDepth == 8;
		}
	}
	return false;
}

img_data read_tga(uint8_t* file_data)
{
	TGAFileHead* fileHead = (TGAFileHead*)file_data;

	if (fileHead->GetColorType() == TGAColorType::IndexedColor)
	{
		img_data image;
		set_img_data_bgr24(image, fileHead->Width, fileHead->Height);

		// locate color table.
		ColorTable24 colorTable;
		auto palettePtr = (BGR24*)(file_data + kSizeTGAFileHead + fileHead->IDLength);
		colorTable.table = palettePtr + fileHead->CMapStart;
		colorTable.count = fileHead->GetColorTableCount();

		// fill scanline datas.
		auto srcPitch = line_size4(fileHead->Width, fileHead->PixelDepth);
		auto dataPtr = file_data + kSizeTGAFileHead + fileHead->IDLength + fileHead->CMapLength * sizeof(BGR24);
		for (uint32_t row = 0; row < image.height; row++)
		{
			for (uint32_t col = 0; col < image.width; col++)
			{
				auto srcPtr = dataPtr + row * srcPitch + col;
				auto dstPtr = image.get_pixel(col, image.height - row - 1);
				auto& index = *srcPtr;
				memcpy(dstPtr, colorTable[index], 3);
			}
		}
		return image;
	}
	else if (fileHead->GetColorType() == TGAColorType::TrueColor)
	{
		img_data image;		
		if (fileHead->PixelDepth == 32)
		{
			set_img_data_bgra32(image, fileHead->Width, fileHead->Height);
		}
		else
		{
			set_img_data_bgr24(image, fileHead->Width, fileHead->Height);
		}

		auto srcPitch = line_size4(fileHead->Width, fileHead->PixelDepth);
		auto dataPtr = file_data + kSizeTGAFileHead + fileHead->IDLength;
		for (uint32_t row = 0; row < image.height; row++)
		{
			auto srcPtr = dataPtr + row * srcPitch;
			auto dstPtr = image.get_row(image.height - row - 1);
			memcpy(dstPtr, srcPtr, image.line_pitch);
		}
		return image;
	}
	else // grayscale
	{
		img_data image;
		set_img_data_bgr24(image, fileHead->Width, fileHead->Height);

		auto srcPitch = line_size4(fileHead->Width, fileHead->PixelDepth);
		auto dataPtr = file_data + kSizeTGAFileHead + fileHead->IDLength;
		for (uint32_t row = 0; row < image.height; row++)
		{
			auto srcPtr = dataPtr + row * srcPitch;
			for (uint32_t col = 0; col < image.width; col++)
			{
				auto dstPtr = image.get_pixel(col, image.height - row - 1);
				dstPtr[2] = dstPtr[1] = dstPtr[0] = srcPtr[col];
			}
		}
		return image;
	}
}