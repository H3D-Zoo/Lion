#include "APIInstance.h"
#include "Device.h"
#include "Context.h"
#include "AutoPtr.hpp"

RenderAPI::APIInstance* CreateAPIInstance()
{
	return new ::APIInstance();
}

APIInstance::APIInstance()
	: m_deviceDriver("null_driver")
	, m_deviceName("null_device")
	, m_deviceDesc("null")
	, m_vendorID(0)
{
	m_driverVersion.Product = 1;;
	m_driverVersion.Version = 1;
	m_driverVersion.Subversion = 1;
	m_driverVersion.BuildNumber = 1;
	m_driverVersion.WHQLLevel = 0;
}

void APIInstance::AddRef()
{
	++m_refCount;
}

void APIInstance::LogError(const char* action, const char * detail)
{
	char buff[512];
	_sprintf_p(buff, 512, "%s Failed, %s, ", action, detail);
	GetCurrentLogger()->Log(RenderAPI::LOG_Error, buff);
}

void APIInstance::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}


RenderAPI::CreationResult APIInstance::CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync)
{
	RenderAPI::CreationResult result;
	if (desc.backbufferWidth == 0 || desc.backbufferHeight == 0)
	{
		return result;
	}
	else
	{
		result.Success = true;

		pDevice = new ::Device(*this, desc, isFullscreen, useVerticalSync);

		AutoR<RenderAPI::SwapChain> swapChain = pDevice->GetDefaultSwapChain();
		AutoR<RenderAPI::RenderTarget> rt = swapChain->GetRenderTarget();
		AutoR<RenderAPI::DepthStencil> ds = swapChain->GetDepthStencil();

		pContext = new ::Context(rt, ds, renderStatistic);

		result.DevicePtr = pDevice;		
		result.ContextPtr = pContext;

		return result;
	}
}

bool APIInstance::CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile, const char* includeDir)
{
	return true;
}

RenderAPI::RenderTargetFormat APIInstance::GetDefaultRenderTargetFormat()
{
	return RenderAPI::RT_XRGB8;
}

const char * APIInstance::GetDeviceDriver() const
{
	return m_deviceDriver.c_str();
}

const char * APIInstance::GetDeviceName() const
{
	return m_deviceName.c_str();
}

const char * APIInstance::GetDeviceDesc() const
{
	return m_deviceDesc.c_str();
}

unsigned int APIInstance::GetVendorID() const
{
	return m_vendorID;
}

bool APIInstance::CheckMultiSampleSupport(RenderAPI::RenderTargetFormat bb, RenderAPI::DepthStencilFormat z, RenderAPI::AAMode aa, bool fullscreen) const
{
	return true;
}

RenderAPI::DriverVersion APIInstance::GetDriverVersion() const
{
	return m_driverVersion;
}

bool APIInstance::IsSupportEx() const 
{
	return true;
}

void APIInstance::PerfBegin(unsigned int color, const char* name)
{

}

void APIInstance::PerfMark(unsigned int color, const char * name)
{

}

void APIInstance::PerfEnd()
{

}