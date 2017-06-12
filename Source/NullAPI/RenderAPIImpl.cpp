#include <stdlib.h>
#include "../../RenderAPI/RenderAPI.h"
#include "Device.h"
#include "Context.h"
#include "RenderAPIImpl.h"


namespace RenderAPI
{
	static Logger* s_currentLogger = NULL;

	bool Initialize()
	{
		return true;
	}

	void Deinitialize()
	{
	}
	
	CreationResult CreateDeviceAndContext(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync)
	{
		CreationResult result;
		result.Success = true;
		result.DevicePtr = new ::Device(desc, isFullscreen, useVerticalSync);
		result.ContextPtr = new ::Context();
		return result;
	}
}

RefCount::RefCount() : count(1) { }
