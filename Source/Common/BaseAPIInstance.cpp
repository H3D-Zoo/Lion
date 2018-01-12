#include "../../RenderAPI/RenderAPI.h"
#include "BaseAPIInstance.h"
#include "Logger.h"


BaseAPIInstance::BaseAPIInstance() : m_currentLogger(GetDefaultLogger())
{

}

RenderAPI::Logger* BaseAPIInstance::GetDefaultLogger() const
{
	return &(SilentLogger::GetInstance());
}

RenderAPI::Logger* BaseAPIInstance::GetCurrentLogger() const
{
	return m_currentLogger;
}

void BaseAPIInstance::SetCurrentLogger(RenderAPI::Logger* logger)
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