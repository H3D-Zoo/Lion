#pragma once
#include <cinttypes>
#include <memory>
struct img_data;

struct BGR24
{
	uint8_t b, g, r;
};
struct BGRA32
{
	uint8_t b, g, r, a;
};

template<class T> struct Table
{
	T* table = nullptr;
	T def_value;
	uint32_t count = 0;

	operator bool() { return table != nullptr && count > 0; }

	T* operator[](uint32_t index) { return index < count ? table + index : &def_value; }

	const T* operator[](uint32_t index) const { return index < count ? return table + index : &def_value; }

	T get(uint32_t index) const { return *operator[](index); }
};

typedef Table<BGR24> ColorTable24;
typedef Table<BGRA32> ColorTable32;
typedef Table<uint8_t> AlphaTable;

inline uint32_t line_size4(uint32_t width, uint32_t bits) { return (width * bits + 31) / 32 * 4; }
inline uint32_t line_size1(uint32_t width, uint32_t bits) { return (width * bits + 7) / 8; }

constexpr uint32_t MakeFourCC(const char fourcc[5])
{
	return (((((fourcc[3] << 8) | fourcc[2]) << 8) | fourcc[1]) << 8) | fourcc[0];
};



bool is_bmp(uint8_t* file_data);
bool is_png(uint8_t* file_data);
bool is_tga(uint8_t* file_data);
bool is_dds(uint8_t* file_data);
img_data read_bmp(uint8_t* file_data);
img_data read_png(uint8_t* file_data);
img_data read_tga(uint8_t* file_data);
img_data read_dds(uint8_t* file_data);
void set_img_data_bgr24(img_data& image, uint32_t width, uint32_t height);
void set_img_data_bgra32(img_data& image, uint32_t width, uint32_t height);
