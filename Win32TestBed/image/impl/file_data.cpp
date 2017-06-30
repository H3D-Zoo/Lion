#include <stdio.h>
#include "../file_data.h"

void file_data::destroy()
{
	if (buffer != nullptr)
	{
		delete buffer;
		buffer = nullptr;
	}
	length = 0;
}

file_data read_file(const char* path)
{
	FILE* file = nullptr;//fopen(path, "wb+");
	if (fopen_s(&file, path, "rb") != 0
		|| file == nullptr)
	{
		return file_data();
	}

	bool result = true;
	uint32_t length = 0;
	uint8_t* buffer = nullptr;
	do
	{
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		if (length == 0)
		{
			result = false;
			break;
		}

		buffer = new uint8_t[length];
		if (buffer == nullptr)
		{
			result = false;
			break;
		}
		fseek(file, 0, SEEK_SET);

		length = static_cast<long>(fread(buffer, 1, length, file));
		if (length == 0)
		{
			delete[] buffer;
			result = false;
			break;
		}
	} while (false);
	fclose(file);

	if (result)
	{
		file_data fileData;
		fileData.buffer = buffer;
		fileData.length = length;
		return fileData;
	}
	else
	{
		return file_data();
	}
}