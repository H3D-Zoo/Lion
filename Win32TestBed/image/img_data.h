#pragma once
#include <cinttypes>

enum class color_format : int
{
	invalid,// no support data.
	bgr24,	// 0xBBGGRR
	bgra32,	// 0xBBGGRRAA
	dxt1,	// DXT1
	dxt2,	// DXT2
	dxt3,	// DXT3
	dxt4,	// DXT4
	dxt5,	// DXT5
};

struct img_data
{
	color_format format = color_format::invalid;
	uint8_t* buffer = nullptr;
	uint32_t length = 0;
	uint32_t pixel_stride = 0;
	uint32_t line_pitch = 0;
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t block_width = 0;
	uint32_t block_height = 0;

	bool is_valid() const;

	void create(uint32_t length);

	void destroy();

	uint8_t* get_pixel(uint32_t column, uint32_t row);

	uint8_t* get_row(uint32_t row);
};

//only support limited bmp/tga/png/dds
img_data read_image(uint8_t* file_data);