#include "DX9Include.h"
#include <string>

D3D9DLL::D3D9DLL()
	: m_hDLL(NULL)
	, m_d3d9ExPtr(NULL)
	, m_d3d9Ptr(NULL)
	, D3DPerfBeginEvent(NULL)
	, D3DPerfEndEvent(NULL)
	, D3DPerfSetMarker(NULL)
{

}

// D3D9EX API CREATION, �ֶ�����d3d9.dll����ȡ�����ж��Ƿ�֧�� [1/25/2013 YiKaiming]
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
		// error:d3d9.dll ���ش���, ��������
		return false;
	}

	//verbose:CheckPointStr("��ú���ָ��.begin");
	D3DPerfBeginEvent = (LPD3DPERF_BeginEvent)GetProcAddress(m_hDLL, "D3DPERF_BeginEvent");
	D3DPerfEndEvent = (LPD3DPERF_EndEvent)GetProcAddress(m_hDLL, "D3DPERF_EndEvent");
	D3DPerfSetMarker = (LPD3DPERF_SetMarker)GetProcAddress(m_hDLL, "D3DPERF_SetMarker");

	// ͨ������Direct3DCreate9Ex�Ƿ������ȷ�ϵ�ǰ����ϵͳ�Ƿ�֧��D3D9 EX
	LPDIRECT3DCREATE9EX Direct3DCreate9ExPtr = NULL;
	LPDIRECT3DCREATE9 Direct3DCreate9Ptr = NULL;
	Direct3DCreate9ExPtr = (LPDIRECT3DCREATE9EX)GetProcAddress(m_hDLL, "Direct3DCreate9Ex");
	Direct3DCreate9Ptr = (LPDIRECT3DCREATE9)GetProcAddress(m_hDLL, "Direct3DCreate9");
	//verbose:CheckPointStr("��ú���ָ��.end");

	if (Direct3DCreate9ExPtr != NULL)
	{
		// ���֧��D3D9EX�������豸������IDirect3D9* ����ָ��
		//CheckPointStr("����d3d9Ex����begin");
		HRESULT hr = Direct3DCreate9ExPtr(D3D_SDK_VERSION, &m_d3d9ExPtr);

		//CheckPointStr("����d3d9Ex����end");
		// ������ܻ�ʧ�ܣ�����Կ���֧��WDDM [2/4/2013 YiKaiming]
		if (FAILED(hr))
		{
			//LogInfo(ENGINE_INIT, OutPut_File, "����d3d9Ex����ʧ�ܣ�����D3D9.begin");
			m_d3d9ExPtr = NULL;
			m_d3d9Ptr = (IDirect3D9*)Direct3DCreate9Ptr(D3D_SDK_VERSION);
			//LogInfo(ENGINE_INIT, OutPut_File, "����d3d9. end");
		}
		else
		{
			m_d3d9Ptr = m_d3d9Ptr;
			//LogInfo(ENGINE_INIT, OutPut_File, "�ɹ�����d3d9Ex����.");
		}
	}
	else
	{
		//CheckPointStr("��ʹ��d3d9Ex,����D3D9����begin");
		m_d3d9Ptr = (IDirect3D9*)Direct3DCreate9Ptr(D3D_SDK_VERSION);
		//CheckPointStr("����d3d9. end");
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
		// ���Ex�����ɹ������d3d9Ptr��Ϊ��ֵͬ
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

IDirect3DDevice9 * D3D9DLL::CreateDevice(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample)
{
	//by sssa2000 20110120
	DWORD MT = D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE;
	DWORD hardwareBehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING | MT;
	DWORD softwareBehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING | MT;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.hDeviceWindow = hWindow;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.Windowed = !isFullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = rtFormat;
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
		// ���֧��D3D9 EX������EX�豸�������ظ���ָ��
		//CheckPointStr("CreateDeviceEx begin,try D3DCREATE_HARDWARE_VERTEXPROCESSING");
		HRESULT hr = m_d3d9ExPtr->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, hardwareBehaviorFlag, &d3dpp, NULL, &devicePtr);
		//CheckPointStr("����Device end");

		if (devicePtr == NULL)
		{
			//CheckPointStr("CreateDeviceEx begin,try D3DCREATE_SOFTWARE_VERTEXPROCESSING");
			hr = m_d3d9ExPtr->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, softwareBehaviorFlag, &d3dpp, NULL, &devicePtr);
			//CheckPointStr("CreateDeviceEx��end");
		}
		return devicePtr;
	}
	else
	{
		IDirect3DDevice9* devicePtr = NULL;
		//CheckPointStr("D3DCREATE_HARDWARE_VERTEXPROCESSING����Device begin");
		m_d3d9Ptr->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, hardwareBehaviorFlag, &d3dpp, &devicePtr);
		//CheckPointStr("����Device end");
		if (devicePtr == NULL)
		{
			//�еļ����Կ�û��Ӳ��TNL
			//CheckPointStr("D3DCREATE_SOFTWARE_VERTEXPROCESSING����Device begin");
			m_d3d9Ptr->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, softwareBehaviorFlag, &d3dpp, &devicePtr);
			//CheckPointStr("����Device end");
		}
		return devicePtr;
	}
}