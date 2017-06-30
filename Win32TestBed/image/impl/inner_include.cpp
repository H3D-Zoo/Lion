#include "../img_data.h"
#include "inner_include.h"

void set_img_data_bgr24(img_data & image, uint32_t width, uint32_t height)
{
	image.format = color_format::bgr24;
	image.width = width;
	image.height = height;
	image.pixel_stride = 3;
	image.line_pitch = image.pixel_stride * image.width;
	image.create(image.line_pitch * image.height);
}

void set_img_data_bgra32(img_data & image, uint32_t width, uint32_t height)
{
	image.format = color_format::bgra32;
	image.width = width;
	image.height = height;
	image.pixel_stride = 4;
	image.line_pitch = image.pixel_stride * image.width;
	image.create(image.line_pitch * image.height);
}