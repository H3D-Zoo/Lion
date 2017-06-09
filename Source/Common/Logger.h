#include "../../RenderAPI/RenderAPI.h"
#include <string>
#include <fstream>

class FileLogger : public RenderAPI::Logger
{
public:
	FileLogger(const std::string& filePath);

	~FileLogger();

	virtual void LogError(const char*);

	virtual void LogWarning(const char*);

	virtual void LogDebug(const char*);

	virtual void LogVerbose(const char*);

private:
	std::ofstream m_hFile;
};

class SilentLogger : public RenderAPI::Logger
{
public:
	static SilentLogger& GetInstance();

	virtual void LogError(const char*) { }

	virtual void LogWarning(const char*) { }

	virtual void LogDebug(const char*) { }

	virtual void LogVerbose(const char*) { }
};