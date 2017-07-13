#include "../../RenderAPI/RenderAPI.h"
#include "BaseAPIGlobal.h"
#include "Logger.h"


BaseAPIGlobal::BaseAPIGlobal() : m_currentLogger(GetDefaultLogger())
{

}

RenderAPI::Logger* BaseAPIGlobal::GetDefaultLogger()
{
	return &(SilentLogger::GetInstance());
}

RenderAPI::Logger* BaseAPIGlobal::GetCurrentLogger()
{
	return m_currentLogger;
}

void BaseAPIGlobal::SetCurrentLogger(RenderAPI::Logger* logger)
{
	if (logger == NULL)
	{
		m_currentLogger = &(SilentLogger::GetInstance());
	}
	else
	{
		m_currentLogger = logger;
	}
}