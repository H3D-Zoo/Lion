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

const char * APIGlobal::GetDeviceDriver() const
{
	return "";
}

const char * APIGlobal::GetDeviceName() const
{
	return "";
}

const char * APIGlobal::GetDeviceDesc() const
{
	return "";
}

unsigned int APIGlobal::GetVendorID() const
{
	return 0;
}

RenderAPI::DriverVersion APIGlobal::GetDriverVersion() const
{
	RenderAPI::DriverVersion version;

	version.Product = 0;
	version.Version = 0;
	version.Subversion = 0;
	version.BuildNumber = 0;
	version.WHQLLevel = 0;
	return version;
}

bool APIGlobal::CheckMultiSampleSupport(RenderAPI::RenderTargetFormat bb, RenderAPI::DepthStencilFormat z, RenderAPI::AAMode aa, bool fullscreen) const 
{ 
	return true;
}
