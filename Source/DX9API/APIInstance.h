#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <ddraw.h>
#include <d3dx9effect.h>
#include <string>
#include "BaseAPIInstance.h"
#include "RefCount.hpp"

typedef int (WINAPI *LPD3DPERF_BeginEvent)(D3DCOLOR col, LPCWSTR wszName);
typedef int (WINAPI *LPD3DPERF_EndEvent)(void);
typedef void (WINAPI *LPD3DPERF_SetMarker)(D3DCOLOR col, LPCWSTR wszName);

class Device;
class Context;

class APIInstance : public BaseAPIInstance
{
public:
	static D3DPRESENT_PARAMETERS FillCreationParam(APIInstance& self, HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample);
	APIInstance();

	bool Init();

	bool IsSupportD3D9EX();

	bool IsSupportOcclusionQuery();

	bool CheckFormatValidate(D3DFORMAT & renderTarget, D3DFORMAT depthStencil) const;

	IDirect3DDevice9* CreateDevice(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample);

	bool IsSupportManaged();

	// 如果在设备停止的时候，D3D对象也要被重新创建
	// 但是我们暂时没能力获取硬件停止的状态
	// 所以这个接口没人用
	void Recreate();

	virtual RenderAPI::CreationResult CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile);

	virtual RenderAPI::RenderTargetFormat GetDefaultRenderTargetFormat();

	virtual const char* GetDeviceDriver() const;

	virtual const char* GetDeviceName() const;

	virtual const char* GetDeviceDesc() const;

	virtual unsigned int GetVendorID() const;

	virtual bool CheckMultiSampleSupport(RenderAPI::RenderTargetFormat, RenderAPI::DepthStencilFormat, RenderAPI::AAMode, bool fullscreen) const;

	virtual RenderAPI::DriverVersion GetDriverVersion() const;

	virtual void PerfBegin(unsigned int color, const char* name);

	virtual void PerfMark(unsigned int color, const char* name);

	virtual void PerfEnd();

	virtual void Release();

	void AddRef();

private:
	void Deinit();

	bool CheckBackBufferFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const;

	bool CheckDepthStencilFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const;

	bool CheckDeviceMultiSampleType(D3DFORMAT rtFormat, D3DFORMAT dsFormat, bool isFullsreen, D3DMULTISAMPLE_TYPE mulsampleType) const;

	void CreateD3D();

	void DestroyD3D();

	HMODULE m_hDLL;
	RefCount m_refCount;

	// 如果Ex创建成功，会把d3d9Ptr设为相同值
	IDirect3D9Ex* m_d3d9ExPtr;
	IDirect3D9* m_d3d9Ptr;
	std::string m_deviceDriver;
	std::string m_deviceName;
	std::string m_deviceDesc;
	unsigned int m_vendorID;
	RenderAPI::DriverVersion m_driverVersion;
	bool m_supportOcclusionQuery;

public:
	// D3D导出函数的实际指针 [2/1/2013 YiKaiming]
	LPD3DPERF_BeginEvent D3DPerfBeginEvent;
	LPD3DPERF_EndEvent   D3DPerfEndEvent;
	LPD3DPERF_SetMarker  D3DPerfSetMarker;

	::Device* pDevice;
	::Context* pContext;
	HWND hDeviceWindow;
};

class EffectInclude : public ID3DXInclude
{
	HRESULT STDMETHODCALLTYPE Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);
};