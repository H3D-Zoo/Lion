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

void FileLogger::Log(RenderAPI::LogLevel level, const char* str)
{
	switch (level)
	{
	case RenderAPI::LOG_Verbose:m_hFile << "verbose:"; break;
	case RenderAPI::LOG_Debug:	m_hFile << "debug:"; break;
	case RenderAPI::LOG_Warning:m_hFile << "warning:"; break;
	case RenderAPI::LOG_Error:	m_hFile << "error:"; break;
	}
	m_hFile << str << std::endl;
}

SilentLogger & SilentLogger::GetInstance()
{
	static SilentLogger instance;
	return instance;
}
