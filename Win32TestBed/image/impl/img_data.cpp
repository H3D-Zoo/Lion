#include "../img_data.h"
#include "inner_include.h"

void img_data::destroy()
{
	if (buffer != nullptr)
	{
		delete[] buffer;
	}
	format = color_format::invalid;
	buffer = nullptr;
	length = 0;
	width = 0;
	height = 0;
	block_width = 0;
	block_height = 0;
}

uint8_t * img_data::get_pixel(uint32_t column, uint32_t row)
{
	if (column > width) column = width;
	if (row > height) row = height;
	return buffer + row * line_pitch + column * pixel_stride;
}

uint8_t * img_data::get_row(uint32_t row)
{
	if (row > height) row = height;
	return buffer + row * line_pitch;
}

bool img_data::is_valid() const
{
	return format != color_format::invalid
		&& buffer != nullptr
		&& length != 0;
}

void img_data::create(uint32_t length)
{
	this->length = length;
	this->buffer = new uint8_t[length];
}

img_data read_image(uint8_t* file_data)
{
	if (is_dds(file_data))
	{
		return read_dds(file_data);
	}
	else if (is_png(file_data))
	{
		return read_png(file_data);
	}
	else if (is_bmp(file_data))
	{
		return read_bmp(file_data);
	}
	else if (is_tga(file_data))
	{
		return read_tga(file_data);
	}
	else
	{
		return img_data();
	}
}