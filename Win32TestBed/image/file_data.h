#pragma once
#include <cinttypes>

struct file_data
{
	uint8_t* buffer = nullptr;
	uint32_t length = 0;

	bool is_valid() const { return buffer != nullptr; }

	void destroy();
};

file_data read_file(const char* path);