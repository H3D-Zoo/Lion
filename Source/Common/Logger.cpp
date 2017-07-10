#include <assert.h>
#include "Logger.h"



FileLogger::FileLogger(const std::string & filePath)
	: m_hFile(filePath.c_str(), std::ios_base::out)
{
	assert(m_hFile);
}

FileLogger::~FileLogger()
{
	m_hFile.close();
}

void FileLogger::LogE(const char* str)
{
	m_hFile << "error:" << str << std::endl;
}

void FileLogger::LogW(const char* str)
{
	m_hFile << "warning:" << str << std::endl;
}

void FileLogger::LogD(const char* str)
{
	m_hFile << "debug:" << str << std::endl;
}

void FileLogger::LogV(const char* str)
{
	m_hFile << "verbos:" << str << std::endl;
}

SilentLogger & SilentLogger::GetInstance()
{
	static SilentLogger instance;
	return instance;
}
