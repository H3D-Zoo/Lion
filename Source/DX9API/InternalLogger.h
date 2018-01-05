#pragma once
#include "DX9Include.h"

enum LogLevel
{
	LOG_Verbose,
	LOG_Debug,
	LOG_Warning,
	LOG_Error,
};
class IInternalLogger
{
public:
	~IInternalLogger() { }

	virtual void LogStr(LogLevel level, const char* desc) = 0;

	virtual void LogStr(LogLevel level, const char* desc, const char* detail) = 0;

	virtual void LogErr(LogLevel level, const char* desc, HRESULT errCode) = 0;

	virtual void LogErr(LogLevel level, const char* desc, const char* detail, HRESULT errCode) = 0;
};

#define LOG_FUNC_NAME(desc) __FUNCTION__ " : " desc
#define LOG_FUNCTION_FAILED(logger,desc) (logger)->LogStr(LOG_Warning,LOG_FUNC_NAME(desc))
#define LOG_FUNCTION_FAILED_INVALID_CALL(logger,desc) (logger)->LogStr(LOG_Warning,LOG_FUNC_NAME("Invalid Call"),desc)
#define LOG_FUNCTION_FAILED_ERRCODE(logger,desc,errCode) (logger)->LogErr(LOG_Warning,LOG_FUNC_NAME(desc),errCode)
#define LOG_FUNCTION_FAILED_DETAIL(logger,desc,detail,errCode) (logger)->LogErr(LOG_Warning,LOG_FUNC_NAME(desc),detail,errCode)

void LogFunctionCall(IInternalLogger& logger, const char* functionName, const char* format, ...);

//#ifdef _DEBUG
#if 1
#define LOG_FUNCTION_CALL(logger) LogFunctionCall(logger,__FUNCTION__,"")
#define LOG_FUNCTION_PARAM(logger,format,...) LogFunctionCall(logger,__FUNCTION__,format,##__VA_ARGS__)
#else
#define LOG_FUNCTION_CALL(logger)
#define LOG_FUNCTION_PARAM(logger,format,...) 
#endif