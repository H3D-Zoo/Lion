#include "../../RenderAPI/RenderAPI.h"
#include "Logger.h"


namespace RenderAPI
{
	static Logger* s_currentLogger = GetDefaultLogger();

	Logger* GetDefaultLogger()
	{
		return &(SilentLogger::GetInstance());
	}

	Logger* GetCurrentLogger()
	{
		return s_currentLogger;
	}

	void SetCurrentLogger(Logger* logger)
	{
		if (logger == NULL)
		{
			s_currentLogger = &(SilentLogger::GetInstance());
		}
		else
		{
			s_currentLogger = logger;
		}
	}
}