#pragma once
#include "DX9Include.h"

class IInternalLogger
{
public:
	~IInternalLogger() { }

	virtual void LogError(const char* action, const char* detail) = 0;

	virtual void LogError(const char* action, const char* detail, HRESULT errorCode) = 0;
};