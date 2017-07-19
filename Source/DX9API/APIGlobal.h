#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <ddraw.h>
#include <d3dx9effect.h>
#include <string>
#include "BaseAPIGlobal.h"
#include "RefCount.hpp"

typedef int (WINAPI *LPD3DPERF_BeginEvent)(D3DCOLOR col, LPCWSTR wszName);
typedef int (WINAPI *LPD3DPERF_EndEvent)(void);
typedef void (WINAPI *LPD3DPERF_SetMarker)(D3DCOLOR col, LPCWSTR wszName);

class Device;
class Context;

class APIGlobal : public BaseAPIGlobal
{
public:
	APIGlobal();

	bool Init();

	bool IsSupportD3D9EX();

	bool IsSupportOcclusionQuery();

	bool CheckFormatValidate(D3DFORMAT & renderTarget, D3DFORMAT depthStencil) const;

	IDirect3DDevice9* CreateDevice(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample);

	D3DPRESENT_PARAMETERS MakeCreationParam(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample);

	bool IsSupportManaged();

	// ������豸ֹͣ��ʱ��D3D����ҲҪ�����´���
	// ����������ʱû������ȡӲ��ֹͣ��״̬
	// ��������ӿ�û����
	void Recreate();

	virtual RenderAPI::CreationResult CreateDeviceAndContext(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile);

	virtual const char* GetDeviceDriver() const;

	virtual const char* GetDeviceName() const;

	virtual const char* GetDeviceDesc() const;

	virtual unsigned int GetVendorID() const;

	virtual bool CheckMultiSampleSupport(RenderAPI::BackBufferFormat, RenderAPI::ZBufferFormat, RenderAPI::AAMode, bool fullscreen) const;

	virtual RenderAPI::DriverVersion GetDriverVersion() const;

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

	// ���Ex�����ɹ������d3d9Ptr��Ϊ��ֵͬ
	IDirect3D9Ex* m_d3d9ExPtr;
	IDirect3D9* m_d3d9Ptr;
	std::string m_deviceDriver;
	std::string m_deviceName;
	std::string m_deviceDesc;
	unsigned int m_vendorID;
	RenderAPI::DriverVersion m_driverVersion;
	bool m_supportOcclusionQuery;

public:
	// D3D����������ʵ��ָ�� [2/1/2013 YiKaiming]
	LPD3DPERF_BeginEvent D3DPerfBeginEvent;
	LPD3DPERF_EndEvent   D3DPerfEndEvent;
	LPD3DPERF_SetMarker  D3DPerfSetMarker;


	::Device* pDevice;
	::Context* pContext;
	D3DPRESENT_PARAMETERS CreationParam;
};

class EffectInclude : public ID3DXInclude
{
	HRESULT STDMETHODCALLTYPE Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);
};