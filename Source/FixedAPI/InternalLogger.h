#pragma once
#include "DX9Include.h"

void LogFunctionCall(RenderAPI::Logger& logger, RenderAPI::LogLevel level, const char* functionName, const char* format, ...);

//#ifdef _DEBUG
#if 1
#define LOG_FUNCTION_CALL(logger,level) LogFunctionCall(logger,level,__FUNCTION__,"object=%X",this)
#define LOG_FUNCTION_V(logger,format,...) LogFunctionCall(logger,RenderAPI::LOG_Verbose,__FUNCTION__,format,##__VA_ARGS__)
#define LOG_FUNCTION_D(logger,format,...) LogFunctionCall(logger,RenderAPI::LOG_Debug,__FUNCTION__,format,##__VA_ARGS__)
#define LOG_FUNCTION_W(logger,format,...) LogFunctionCall(logger,RenderAPI::LOG_Warning,__FUNCTION__,format,##__VA_ARGS__)
#define LOG_FUNCTION_E(logger,format,...) LogFunctionCall(logger,RenderAPI::LOG_Error,__FUNCTION__,format,##__VA_ARGS__)
#else
#define LOG_FUNCTION_CALL(logger) 
#define LOG_FUNCTION_V(logger,format,...) 
#define LOG_FUNCTION_D(logger,format,...) 
#define LOG_FUNCTION_W(logger,format,...) LogFunctionCall(logger,LOG_Warning,__FUNCTION__,format,##__VA_ARGS__)
#define LOG_FUNCTION_E(logger,format,...) LogFunctionCall(logger,LOG_Error,__FUNCTION__,format,##__VA_ARGS__)
#endif