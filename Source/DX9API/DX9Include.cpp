#include "DX9Include.h"
#include <string>
#include "RenderAPIImpl.h"

D3D9DLL::D3D9DLL()
	: m_hDLL(NULL)
	, m_d3d9ExPtr(NULL)
	, m_d3d9Ptr(NULL)
	, D3DPerfBeginEvent(NULL)
	, D3DPerfEndEvent(NULL)
	, D3DPerfSetMarker(NULL)
{

}

// D3D9EX API CREATION, 手动加载d3d9.dll来获取，以判断是否支持 [1/25/2013 YiKaiming]
// Define a function pointer to the Direct3DCreate9Ex function.
typedef HRESULT(WINAPI *LPDIRECT3DCREATE9EX)(UINT, IDirect3D9Ex**);
typedef IDirect3D9* (WINAPI *LPDIRECT3DCREATE9)(UINT);

bool D3D9DLL::Init()
{
	//  [1/25/2013 YiKaiming]
	// verbose:load d3d9.dll manmually
	m_hDLL = LoadLibraryA("d3d9.dll");
	if (m_hDLL == NULL)
	{
		// error:d3d9.dll 加载错误, 致命错误
		return false;
	}

	//verbose:CheckPointStr("获得函数指针.begin");
	D3DPerfBeginEvent = (LPD3DPERF_BeginEvent)GetProcAddress(m_hDLL, "D3DPERF_BeginEvent");
	D3DPerfEndEvent = (LPD3DPERF_EndEvent)GetProcAddress(m_hDLL, "D3DPERF_EndEvent");
	D3DPerfSetMarker = (LPD3DPERF_SetMarker)GetProcAddress(m_hDLL, "D3DPERF_SetMarker");

	// 通过查找Direct3DCreate9Ex是否存在来确认当前操作系统是否支持D3D9 EX
	LPDIRECT3DCREATE9EX Direct3DCreate9ExPtr = NULL;
	LPDIRECT3DCREATE9 Direct3DCreate9Ptr = NULL;
	Direct3DCreate9ExPtr = (LPDIRECT3DCREATE9EX)GetProcAddress(m_hDLL, "Direct3DCreate9Ex");
	Direct3DCreate9Ptr = (LPDIRECT3DCREATE9)GetProcAddress(m_hDLL, "Direct3DCreate9");
	//verbose:CheckPointStr("获得函数指针.end");

	if (Direct3DCreate9ExPtr != NULL)
	{
		// 如果支持D3D9EX，创建设备并赋予IDirect3D9* 父类指针
		//CheckPointStr("创建d3d9Ex对象，begin");
		HRESULT hr = Direct3DCreate9ExPtr(D3D_SDK_VERSION, &m_d3d9ExPtr);

		//CheckPointStr("创建d3d9Ex对象，end");
		// 这里可能会失败！如果显卡不支持WDDM [2/4/2013 YiKaiming]
		if (FAILED(hr))
		{
			//LogInfo(ENGINE_INIT, OutPut_File, "创建d3d9Ex对象失败，改用D3D9.begin");
			m_d3d9ExPtr = NULL;
			m_d3d9Ptr = (IDirect3D9*)Direct3DCreate9Ptr(D3D_SDK_VERSION);
			//LogInfo(ENGINE_INIT, OutPut_File, "创建d3d9. end");
		}
		else
		{
			m_d3d9Ptr = m_d3d9ExPtr;
			//LogInfo(ENGINE_INIT, OutPut_File, "成功创建d3d9Ex对象.");
		}
	}
	else
	{
		//CheckPointStr("不使用d3d9Ex,创建D3D9对象，begin");
		m_d3d9Ptr = (IDirect3D9*)Direct3DCreate9Ptr(D3D_SDK_VERSION);
		//CheckPointStr("创建d3d9. end");
	}
	if (m_d3d9Ptr == NULL)
	{
		return false;
	}

	return true;
}

void D3D9DLL::Deinit()
{
	if (m_d3d9ExPtr != NULL)
	{
		// 如果Ex创建成功，会把d3d9Ptr设为相同值
		m_d3d9ExPtr->Release();
		m_d3d9ExPtr = NULL;
		m_d3d9Ptr = NULL;
	}
	else if (m_d3d9Ptr != NULL)
	{
		m_d3d9Ptr->Release();
		m_d3d9Ptr = NULL;
	}

	::FreeLibrary(m_hDLL);
}

bool D3D9DLL::IsSupportD3D9EX()
{
	return m_d3d9ExPtr != NULL;
}

bool D3D9DLL::CheckFormatValidate(D3DFORMAT & renderTarget, D3DFORMAT depthStencil) const
{
	D3DDISPLAYMODE d3ddm;
	m_d3d9Ptr->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	if (!CheckBackBufferFormat(renderTarget, d3ddm.Format))
	{
		//warning:
		return false;
	}

	if (!CheckDepthStencilFormat(depthStencil, d3ddm.Format))
	{
		//warning:
		return false;
	}
	return true;
}

bool D3D9DLL::CheckBackBufferFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const
{
	HRESULT hr = m_d3d9Ptr->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		adapterFormat,
		D3DUSAGE_RENDERTARGET,
		D3DRTYPE_SURFACE,
		checkFormat);

	return hr == S_OK;
}

bool D3D9DLL::CheckDepthStencilFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const
{
	HRESULT hr = m_d3d9Ptr->CheckDeviceFormat(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		adapterFormat,
		D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE,
		checkFormat);

	return hr == S_OK;
}


bool D3D9DLL::CheckDeviceMultiSampleType(D3DFORMAT rtFormat, D3DFORMAT dsFormat, bool isFullsreen, D3DMULTISAMPLE_TYPE mulsampleType) const
{
	DWORD numAAQuality;
	HRESULT hr = m_d3d9Ptr->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, rtFormat, !isFullsreen, mulsampleType, &numAAQuality);
	if (hr != S_OK)
	{
		return false;
	}

	hr = m_d3d9Ptr->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dsFormat, !isFullsreen, mulsampleType, &numAAQuality);
	if (hr != S_OK)
	{
		return false;
	}
	return true;
}

D3DPRESENT_PARAMETERS D3D9DLL::MakeCreationParam(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample)
{
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow = hWindow;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.Windowed = !isFullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = rtFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = dsFormat;
	d3dpp.PresentationInterval = vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = mulsample;
	d3dpp.MultiSampleQuality = 0;

	if (CheckDeviceMultiSampleType(rtFormat, dsFormat, isFullscreen, mulsample))
	{
		d3dpp.Flags &= ~D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}
	else
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	return d3dpp;
}

bool D3D9DLL::IsSupportManaged()
{
	return !IsSupportD3D9EX();
}

IDirect3DDevice9 * D3D9DLL::CreateDevice(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample)
{
	//by sssa2000 20110120
	DWORD MT = D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE;
	DWORD hardwareBehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING | MT;
	DWORD softwareBehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING | MT;

	D3DPRESENT_PARAMETERS d3dpp = MakeCreationParam(hWindow, width, height, isFullscreen, vsync, rtFormat, dsFormat, mulsample);
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow = hWindow;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.Windowed = !isFullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = rtFormat;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = dsFormat;
	d3dpp.PresentationInterval = vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.MultiSampleType = mulsample;
	d3dpp.MultiSampleQuality = 0;

	if (CheckDeviceMultiSampleType(rtFormat, dsFormat, isFullscreen, mulsample))
	{
		d3dpp.Flags &= ~D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}
	else
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	if (IsSupportD3D9EX())
	{
		IDirect3DDevice9Ex* devicePtr = NULL;
		// 如果支持D3D9 EX，创建EX设备，并赋回父类指针
		//CheckPointStr("CreateDeviceEx begin,try D3DCREATE_HARDWARE_VERTEXPROCESSING");
		HRESULT hr = m_d3d9ExPtr->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, hardwareBehaviorFlag, &d3dpp, NULL, &devicePtr);
		//CheckPointStr("创建Device end");

		if (devicePtr == NULL)
		{
			//CheckPointStr("CreateDeviceEx begin,try D3DCREATE_SOFTWARE_VERTEXPROCESSING");
			hr = m_d3d9ExPtr->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, softwareBehaviorFlag, &d3dpp, NULL, &devicePtr);
			//CheckPointStr("CreateDeviceEx，end");
		}
		return devicePtr;
	}
	else
	{
		IDirect3DDevice9* devicePtr = NULL;
		//CheckPointStr("D3DCREATE_HARDWARE_VERTEXPROCESSING创建Device begin");
		m_d3d9Ptr->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, hardwareBehaviorFlag, &d3dpp, &devicePtr);
		//CheckPointStr("创建Device end");
		if (devicePtr == NULL)
		{
			//有的集成显卡没有硬件TNL
			//CheckPointStr("D3DCREATE_SOFTWARE_VERTEXPROCESSING创建Device begin");
			m_d3d9Ptr->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, softwareBehaviorFlag, &d3dpp, &devicePtr);
			//CheckPointStr("创建Device end");
		}
		return devicePtr;
	}
}

HRESULT STDMETHODCALLTYPE EffectInclude::Open(D3DXINCLUDE_TYPE, LPCSTR pFileName, LPCVOID /*pParentData*/, LPCVOID *ppData, UINT *pBytes)
{
	if (!IsLocalFileExist(pFileName))
	{
		return E_INVALIDARG;
	}

	HANDLE hFile = CreateFileA(pFileName,		// file to open
		GENERIC_READ,							// open for reading
		FILE_SHARE_READ,						// share for reading
		NULL,									// default security
		OPEN_EXISTING,							// existing file only
		FILE_ATTRIBUTE_NORMAL,					// normal file
		NULL);									// no attr. template

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD FileSize = 0;
		FileSize = GetFileSize(hFile, 0);
		//分配空间
		char* pFileBuffer = new char[FileSize];
		DWORD readByte = 0;
		BOOL readRes = ReadFile(hFile, &(pFileBuffer[0]), FileSize, &readByte, 0);
		if (!readRes)
		{
			int errCode = GetLastError();
			//LogErrorWithDesc("Read Include File 失败%s. pFileName=%s;dest_filename=%s;%d=", pFileName, dest_filename.c_str(), errcode);
			*ppData = 0;
			CloseHandle(hFile);
			return S_FALSE;
		}
		else
		{
			*ppData = pFileBuffer;
			*pBytes = readByte;
			CloseHandle(hFile);
			return S_OK;
		}
	}
	else
	{
		*ppData = NULL;
		*pBytes = 0;
		//char buff[MAX_PATH];
		//GetCurrentDirectoryA(MAX_PATH, buff);
		//g_Log.OutPutConsole(true,"无法打开Fx Include文件%s.\n当前目录:%s",pFileName,buff);
		//LogErrorWithDesc("无法打开Fx 文件dest_filename=%s.\n当前目录:%s ;pFileName=%s", dest_filename.c_str(), buff, pFileName);
		return S_FALSE;
	}
}

HRESULT STDMETHODCALLTYPE EffectInclude::Close(LPCVOID pData)
{
	delete[] pData;
	return S_OK;
}