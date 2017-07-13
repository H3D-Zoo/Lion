#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "BaseAPIGlobal.h"

class APIGlobal : public BaseAPIGlobal
{
public:
	virtual void Release();

	virtual RenderAPI::CreationResult CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	// 从文件中读取fx代码并编译，结果输出到compiledFXFile里
	virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile);
};