#include "../img_data.h"
#include "inner_include.h"

enum class DDSColorType : int
{
	Unknown, DXT1, DXT2, DXT3, DXT4, DXT5, DX10, BGR, BGRA
};

enum class DXGI_FORMAT : uint32_t
{
	DXGI_FORMAT_UNKNOWN = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT = 2,
	DXGI_FORMAT_R32G32B32A32_UINT = 3,
	DXGI_FORMAT_R32G32B32A32_SINT = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS = 5,
	DXGI_FORMAT_R32G32B32_FLOAT = 6,
	DXGI_FORMAT_R32G32B32_UINT = 7,
	DXGI_FORMAT_R32G32B32_SINT = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM = 11,
	DXGI_FORMAT_R16G16B16A16_UINT = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM = 13,
	DXGI_FORMAT_R16G16B16A16_SINT = 14,
	DXGI_FORMAT_R32G32_TYPELESS = 15,
	DXGI_FORMAT_R32G32_FLOAT = 16,
	DXGI_FORMAT_R32G32_UINT = 17,
	DXGI_FORMAT_R32G32_SINT = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM = 24,
	DXGI_FORMAT_R10G10B10A2_UINT = 25,
	DXGI_FORMAT_R11G11B10_FLOAT = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
	DXGI_FORMAT_R8G8B8A8_UINT = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM = 31,
	DXGI_FORMAT_R8G8B8A8_SINT = 32,
	DXGI_FORMAT_R16G16_TYPELESS = 33,
	DXGI_FORMAT_R16G16_FLOAT = 34,
	DXGI_FORMAT_R16G16_UNORM = 35,
	DXGI_FORMAT_R16G16_UINT = 36,
	DXGI_FORMAT_R16G16_SNORM = 37,
	DXGI_FORMAT_R16G16_SINT = 38,
	DXGI_FORMAT_R32_TYPELESS = 39,
	DXGI_FORMAT_D32_FLOAT = 40,
	DXGI_FORMAT_R32_FLOAT = 41,
	DXGI_FORMAT_R32_UINT = 42,
	DXGI_FORMAT_R32_SINT = 43,
	DXGI_FORMAT_R24G8_TYPELESS = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT = 47,
	DXGI_FORMAT_R8G8_TYPELESS = 48,
	DXGI_FORMAT_R8G8_UNORM = 49,
	DXGI_FORMAT_R8G8_UINT = 50,
	DXGI_FORMAT_R8G8_SNORM = 51,
	DXGI_FORMAT_R8G8_SINT = 52,
	DXGI_FORMAT_R16_TYPELESS = 53,
	DXGI_FORMAT_R16_FLOAT = 54,
	DXGI_FORMAT_D16_UNORM = 55,
	DXGI_FORMAT_R16_UNORM = 56,
	DXGI_FORMAT_R16_UINT = 57,
	DXGI_FORMAT_R16_SNORM = 58,
	DXGI_FORMAT_R16_SINT = 59,
	DXGI_FORMAT_R8_TYPELESS = 60,
	DXGI_FORMAT_R8_UNORM = 61,
	DXGI_FORMAT_R8_UINT = 62,
	DXGI_FORMAT_R8_SNORM = 63,
	DXGI_FORMAT_R8_SINT = 64,
	DXGI_FORMAT_A8_UNORM = 65,
	DXGI_FORMAT_R1_UNORM = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM = 69,
	DXGI_FORMAT_BC1_TYPELESS = 70,
	DXGI_FORMAT_BC1_UNORM = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB = 72,
	DXGI_FORMAT_BC2_TYPELESS = 73,
	DXGI_FORMAT_BC2_UNORM = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB = 75,
	DXGI_FORMAT_BC3_TYPELESS = 76,
	DXGI_FORMAT_BC3_UNORM = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB = 78,
	DXGI_FORMAT_BC4_TYPELESS = 79,
	DXGI_FORMAT_BC4_UNORM = 80,
	DXGI_FORMAT_BC4_SNORM = 81,
	DXGI_FORMAT_BC5_TYPELESS = 82,
	DXGI_FORMAT_BC5_UNORM = 83,
	DXGI_FORMAT_BC5_SNORM = 84,
	DXGI_FORMAT_B5G6R5_UNORM = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
	DXGI_FORMAT_BC6H_TYPELESS = 94,
	DXGI_FORMAT_BC6H_UF16 = 95,
	DXGI_FORMAT_BC6H_SF16 = 96,
	DXGI_FORMAT_BC7_TYPELESS = 97,
	DXGI_FORMAT_BC7_UNORM = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB = 99,
	DXGI_FORMAT_AYUV = 100,
	DXGI_FORMAT_Y410 = 101,
	DXGI_FORMAT_Y416 = 102,
	DXGI_FORMAT_NV12 = 103,
	DXGI_FORMAT_P010 = 104,
	DXGI_FORMAT_P016 = 105,
	DXGI_FORMAT_420_OPAQUE = 106,
	DXGI_FORMAT_YUY2 = 107,
	DXGI_FORMAT_Y210 = 108,
	DXGI_FORMAT_Y216 = 109,
	DXGI_FORMAT_NV11 = 110,
	DXGI_FORMAT_AI44 = 111,
	DXGI_FORMAT_IA44 = 112,
	DXGI_FORMAT_P8 = 113,
	DXGI_FORMAT_A8P8 = 114,
	DXGI_FORMAT_B4G4R4A4_UNORM = 115,
	DXGI_FORMAT_P208 = 130,
	DXGI_FORMAT_V208 = 131,
	DXGI_FORMAT_V408 = 132,
	DXGI_FORMAT_FORCE_UINT = 0xffffffff
};

enum class D3D10_RESOURCE_DIMENSION : int
{
	D3D10_RESOURCE_DIMENSION_TEXTURE3D = 4,
	D3D10_RESOURCE_DIMENSION_UNKNOWN = 0,
	D3D10_RESOURCE_DIMENSION_BUFFER = 1,
	D3D10_RESOURCE_DIMENSION_TEXTURE1D = 2,
	D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3,
};

constexpr uint32_t kDDSMagic = 0x20534444; //'DDS'

constexpr uint32_t kDDSDCaps = 0x00000001;
constexpr uint32_t kDDSDHeight = 0x00000002;
constexpr uint32_t kDDSDWidth = 0x00000004;
constexpr uint32_t kDDSDPitch = 0x00000008;
constexpr uint32_t kDDSDPixelFormat = 0x00001000;
constexpr uint32_t kDDSDMipmapCount = 0x00020000;
constexpr uint32_t kDDSDLinearSize = 0x00080000;
constexpr uint32_t kDDSDDepth = 0x00800000;

constexpr uint32_t DDSCapsComplex = 0x00000008;
constexpr uint32_t DDSCapsMipmap = 0x00400000;
constexpr uint32_t DDSCapsTexture = 0x00001000;

constexpr uint32_t kDDPFHaveAlpha = 0x00000001;
constexpr uint32_t kDDPFAlphaChannel = 0x00000002;	//Alpha uncompressed data
constexpr uint32_t kDDPFUseFourCC = 0x00000004;	// contains compressed RGB
constexpr uint32_t kDDPFRGBChannel = 0x00000040;	// contains uncompressed RGB data
constexpr uint32_t kDDPFYUVChannel = 0x00000200;	// YUV uncompressed data
constexpr uint32_t kDDPFLumChannel = 0x00020000;// uncompressed data, Can be combined with AlphaChannel for a two channel.

#define FourCC(x) constexpr const uint32_t DDS_##x = MakeFourCC(#x);
FourCC(DXT1);
FourCC(DXT2);
FourCC(DXT3);
FourCC(DXT4);
FourCC(DXT5);
FourCC(DX10);
#undef FourCC

#pragma pack(push)
#pragma pack(1)
struct DDSPixelFormat
{
	uint32_t Size;
	uint32_t Flags;
	uint32_t FourCC;
	uint32_t RGBBitCount;
	uint32_t RBitMask;
	uint32_t GBitMask;
	uint32_t BBitMask;
	uint32_t ABitMask;

	bool UseFourCC() const { return (Flags & kDDPFUseFourCC) > 0; }
	bool HaveAlpha() const { return (Flags & kDDPFHaveAlpha) > 0; }
	bool HaveAlphaChannel() const { return (Flags & kDDPFAlphaChannel) > 0; }
	bool HaveRGBChannel() const { return (Flags & kDDPFRGBChannel) > 0; }
	bool HaveYUVChannel() const { return (Flags & kDDPFYUVChannel) > 0; }
	bool HaveLumChannel() const { return (Flags & kDDPFLumChannel) > 0; }
};

struct DDSFileHead
{
	uint32_t Size;
	uint32_t Flags;
	uint32_t Height;
	uint32_t Width;
	uint32_t PitchOrLinearSize;
	uint32_t Depth;
	uint32_t MipMapCount;
	uint32_t Reserved1[11];
	DDSPixelFormat ddspf;
	uint32_t Caps;
	uint32_t Caps2;
	uint32_t Caps3;
	uint32_t Caps4;
	uint32_t Reserved2;

	bool MultiSurface() const { return (Caps & DDSCapsComplex) > 0; }
	bool UsedForMipmap() const { return (Caps & DDSCapsMipmap) > 0; }

	bool HavePitch() const { return (Flags & kDDSDPitch) > 0; }
	bool HaveMipmapCount() const { return (Flags & kDDSDMipmapCount) > 0; }
	bool HaveLinearSize() const { return (Flags & kDDSDLinearSize) > 0; }
	bool HaveDepth() const { return (Flags & kDDSDDepth) > 0; }

	bool HaveMipmaps()
	{
		return HaveMipmapCount()
			&& MipMapCount > 1
			&& MultiSurface();
	}

	DDSColorType GetColorFormat()
	{
		if (ddspf.UseFourCC())
		{
			//compressed
			if (ddspf.FourCC == DDS_DXT1)
			{
				return DDSColorType::DXT1;
			}
			else if (ddspf.FourCC == DDS_DXT2)
			{
				return DDSColorType::DXT2;
			}
			else if (ddspf.FourCC == DDS_DXT3)
			{
				return DDSColorType::DXT3;
			}
			else if (ddspf.FourCC == DDS_DXT4)
			{
				return DDSColorType::DXT4;
			}
			else if (ddspf.FourCC == DDS_DXT5)
			{
				return DDSColorType::DXT5;
			}
			else if (ddspf.FourCC == DDS_DX10)
			{
				return DDSColorType::DX10;
			}
			else
			{
				return DDSColorType::Unknown;
			}
		}
		else if (ddspf.HaveRGBChannel())
		{
			//uncompressed rgb
			return (ddspf.HaveAlpha()) ? DDSColorType::BGRA : DDSColorType::BGR;
		}
		else
		{
			return DDSColorType::Unknown;
		}
	}
};

struct DDSDX10Head
{
	DXGI_FORMAT dxgiFormat;
	D3D10_RESOURCE_DIMENSION resourceDimension;
	uint32_t miscFlag;
	uint32_t arraySize;
	uint32_t miscFlags2;
};

#pragma pack(pop)

bool is_dds(uint8_t* file_data)
{
	auto ddsMagic = (uint32_t*)file_data;
	if (*ddsMagic == kDDSMagic)
	{
		auto ddsHeader = (DDSFileHead*)(file_data + sizeof(kDDSMagic));
		auto format = ddsHeader->GetColorFormat();

		return (format != DDSColorType::Unknown && format != DDSColorType::DX10);
	}
	return false;
}

img_data read_dds(uint8_t* file_data)
{
	file_data += sizeof(kDDSMagic);
	auto ddsHeader = (DDSFileHead*)file_data;
	auto format = ddsHeader->GetColorFormat();
	/*
	The D3DX library (for example, D3DX11.lib) and other similar libraries unreliably or
	"INCONSISTENTLY" provide the pitch value in the dwPitchOrLinearSize member of the DDS_HEADER structure.
	Therefore, when you read and write to DDS files,
	we recommend that you compute the pitch in one of the following ways for the indicated formats:

	- For block-compressed formats, compute the pitch as:
		  max( 1, ((width+3)/4) ) * block-size
	  The block-size is 8 bytes for DXT1, BC1, and BC4 formats, and 16 bytes for other block-compressed formats.

	- For R8G8_B8G8, G8R8_G8B8, legacy UYVY-packed,
	  and legacy YUY2-packed formats, compute the pitch as:
		  ((width+1) >> 1) * 4

	- For other formats, compute the pitch as:
		  ( width * bits-per-pixel + 7 ) / 8
	  You divide by 8 for byte alignment.

	**Note** The pitch value that you calculate "DOES NOT"
	always equal the pitch that the runtime supplies,
	which is DWORD-aligned in some situations and byte-aligned in other situations.
	Therefore, we recommend that you copy a scan line at a time rather than try to copy the whole image in one copy.
	*/
	img_data image;
	image.width = ddsHeader->Width;
	image.height = ddsHeader->Height;
	bool isCompressed = false;
	uint32_t srcColorSpace;
	if (format == DDSColorType::DXT1)
	{
		image.block_width = __max(1, ((image.width + 3) / 4));
		image.block_height = __max(1, ((image.height + 3) / 4));
		image.pixel_stride = 8;
		image.line_pitch = image.block_width * image.pixel_stride;
		image.format = color_format::dxt1;
		isCompressed = true;
	}
	else if (format == DDSColorType::DXT2
		|| format == DDSColorType::DXT3
		|| format == DDSColorType::DXT4
		|| format == DDSColorType::DXT5)
	{
		image.block_width = __max(1, ((image.width + 3) / 4));
		image.block_height = __max(1, ((image.height + 3) / 4));
		image.pixel_stride = 16;
		image.line_pitch = image.block_width * image.pixel_stride;
		isCompressed = true;

		switch (format)
		{
		case DDSColorType::DXT2: image.format = color_format::dxt2; break;
		case DDSColorType::DXT3: image.format = color_format::dxt3; break;
		case DDSColorType::DXT4: image.format = color_format::dxt4; break;
		case DDSColorType::DXT5: image.format = color_format::dxt5; break;
		}
	}
	else if (format == DDSColorType::BGR)
	{
		image.line_pitch = line_size1(image.width, 24);
		image.format = color_format::bgr24;
		image.pixel_stride = 3;
		if (ddsHeader->ddspf.RGBBitCount == 32)
		{
			srcColorSpace = 4;
		}
		else if (ddsHeader->ddspf.RGBBitCount == 24)
		{
			srcColorSpace = 3;
		}
		else
		{
			//error
			return img_data();
		}
	}
	else if (format == DDSColorType::BGRA)
	{
		image.line_pitch = line_size1(image.width, 32);
		image.format = color_format::bgra32;
		image.pixel_stride = 4;
		if (ddsHeader->ddspf.RGBBitCount == 32)
		{
			srcColorSpace = 4;
		}
		else
		{
			//error
			return img_data();
		}
	}

	file_data += sizeof(DDSFileHead);
	auto dataBlockPtr = file_data;

	if (isCompressed)
	{
		image.create(image.line_pitch * image.block_height);
		for (uint32_t row = 0; row < image.block_height; row++)
		{
			auto srcOffset = dataBlockPtr + row * image.line_pitch;
			auto dstOffset = image.buffer + row * image.line_pitch;
			memcpy(dstOffset, srcOffset, image.line_pitch);
		}
	}
	else if (srcColorSpace == image.pixel_stride)
	{
		image.create(image.line_pitch * image.height);
		auto srcPitch = image.line_pitch;
		for (uint32_t row = 0; row < image.height; row++)
		{
			auto srcOffset = dataBlockPtr + row * srcPitch;
			auto dstOffset = image.get_row(row);
			memcpy(dstOffset, srcOffset, image.line_pitch);
		}
	}
	else if (srcColorSpace > image.pixel_stride)
	{
		image.create(image.line_pitch * image.height);
		auto srcPitch = line_size1(image.width, srcColorSpace * 8);
		for (uint32_t row = 0; row < image.height; row++)
		{
			for (uint32_t col = 0; col < image.width; col++)
			{
				auto srcOffset = dataBlockPtr + row * srcPitch + col * srcColorSpace;
				auto dstOffset = image.get_pixel(col, row);
				memcpy(dstOffset, srcOffset, image.pixel_stride);
			}
		}
	}
	else
	{
		return img_data();
	}

	return image;
}