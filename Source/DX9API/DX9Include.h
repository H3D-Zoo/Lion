#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <ddraw.h>
#include <d3dx9effect.h>

typedef int (WINAPI *LPD3DPERF_BeginEvent)(D3DCOLOR col, LPCWSTR wszName);
typedef int (WINAPI *LPD3DPERF_EndEvent)(void);
typedef void (WINAPI *LPD3DPERF_SetMarker)(D3DCOLOR col, LPCWSTR wszName);

class D3D9DLL
{
public:
	D3D9DLL();

	bool Init();

	void Deinit();

	bool IsSupportD3D9EX();

	bool CheckFormatValidate(D3DFORMAT & renderTarget, D3DFORMAT depthStencil) const;

	IDirect3DDevice9* CreateDevice(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample);

	D3DPRESENT_PARAMETERS MakeCreationParam(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample);

	bool IsSupportManaged();

private:
	bool CheckBackBufferFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const;

	bool CheckDepthStencilFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const;

	bool CheckDeviceMultiSampleType(D3DFORMAT rtFormat, D3DFORMAT dsFormat, bool isFullsreen, D3DMULTISAMPLE_TYPE mulsampleType) const;

	HMODULE m_hDLL;

	// 如果Ex创建成功，会把d3d9Ptr设为相同值
	IDirect3D9Ex* m_d3d9ExPtr;
	IDirect3D9* m_d3d9Ptr;

public:
	// D3D导出函数的实际指针 [2/1/2013 YiKaiming]
	LPD3DPERF_BeginEvent D3DPerfBeginEvent;
	LPD3DPERF_EndEvent   D3DPerfEndEvent;
	LPD3DPERF_SetMarker  D3DPerfSetMarker;
};

class EffectInclude : public ID3DXInclude
{
	HRESULT STDMETHODCALLTYPE Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData);
};