#include "../../RenderAPI/RenderAPI.h"
#include <string>
#include <fstream>

class FileLogger : public RenderAPI::Logger
{
public:
	FileLogger(const std::string& filePath);

	~FileLogger();

	virtual void Log(RenderAPI::LogLevel, const char*);

private:
	std::ofstream m_hFile;
};

class SilentLogger : public RenderAPI::Logger
{
public:
	static SilentLogger& GetInstance();

	virtual void Log(RenderAPI::LogLevel, const char*) { }
};