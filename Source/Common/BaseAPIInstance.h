#pragma once
#include "../../RenderAPI/RenderAPI.h"

class BaseAPIInstance : public RenderAPI::APIInstance
{
public:
	BaseAPIInstance();

	// 获得系统内置默认Log对象
	virtual RenderAPI::Logger* GetDefaultLogger();

	// 获取当前系统正在使用的Log对象
	virtual RenderAPI::Logger* GetCurrentLogger();

	// 修改系统使用的Log对象，以自定义log输出位置。
	// 当参数为nullptr的时候，系统会使用默认的log对象处理输出
	virtual void SetCurrentLogger(RenderAPI::Logger*);

protected:
	RenderAPI::Logger* m_currentLogger;
};
