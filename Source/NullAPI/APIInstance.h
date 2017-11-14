#pragma once

#include <string>
#include "BaseAPIInstance.h"
#include "InternalLogger.h"
#include "RefCount.hpp"
#include "RenderStatistic.h"

class Device;
class Context;
class APIInstance : public BaseAPIInstance, public IInternalLogger
{
public:
	APIInstance();

	virtual RenderAPI::CreationResult CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile, const char* includeDir);

	virtual RenderAPI::RenderTargetFormat GetDefaultRenderTargetFormat();

	virtual const char* GetDeviceDriver() const;

	virtual const char* GetDeviceName() const;

	virtual const char* GetDeviceDesc() const;

	virtual unsigned int GetVendorID() const;

	virtual bool CheckMultiSampleSupport(RenderAPI::RenderTargetFormat, RenderAPI::DepthStencilFormat, RenderAPI::AAMode, bool fullscreen) const;

	virtual RenderAPI::DriverVersion GetDriverVersion() const;

	virtual bool IsSupportEx() const;

	virtual void PerfBegin(unsigned int color, const char* name);

	virtual void PerfMark(unsigned int color, const char* name);

	virtual void PerfEnd();

	virtual void Release();

	void AddRef();

	void LogError(const char* action, const char* detail);
private:

	RefCount m_refCount;
	const std::string m_deviceDriver;
	const std::string m_deviceName;
	const std::string m_deviceDesc;
	const unsigned int m_vendorID;
	RenderAPI::DriverVersion m_driverVersion;

public:
	::Device* pDevice;
	::Context* pContext;
	RenderStatistic renderStatistic;
};