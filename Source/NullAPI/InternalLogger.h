#pragma once

class IInternalLogger
{
public:
	virtual ~IInternalLogger(){ }

	virtual void LogError(const char* action, const char* detail) = 0;
};
