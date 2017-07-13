#include "APIGlobal.h"
#include <stdlib.h>
#include "../../RenderAPI/RenderAPI.h"
#include "Device.h"
#include "Context.h"


namespace RenderAPI
{
	RenderAPI::APIGlobal* CreateAPIGlobal()
	{
		return new ::APIGlobal();
	}
}

void APIGlobal::Release()
{
	delete this;
}


RenderAPI::CreationResult APIGlobal::CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync)
{
	RenderAPI::CreationResult result;
	result.Success = true;
	result.DevicePtr = new ::Device(desc, isFullscreen, useVerticalSync);
	result.ContextPtr = new ::Context();
	return result;
}

bool APIGlobal::CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile)
{
	return true;
}