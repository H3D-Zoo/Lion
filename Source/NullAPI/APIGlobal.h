#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "BaseAPIGlobal.h"

class APIGlobal : public BaseAPIGlobal
{
public:
	virtual void Release();

	virtual RenderAPI::CreationResult CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	// ���ļ��ж�ȡfx���벢���룬��������compiledFXFile��
	virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile);

	virtual const char* GetDeviceDriver() const;

	virtual const char* GetDeviceName() const;

	virtual const char* GetDeviceDesc() const;

	virtual unsigned int GetVendorID() const;

	virtual RenderAPI::DriverVersion GetDriverVersion() const;

	virtual bool CheckMultiSampleSupport(RenderAPI::BackBufferFormat bb, RenderAPI::ZBufferFormat z, RenderAPI::AAMode aa, bool fullscreen) const;
};