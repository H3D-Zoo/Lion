#include "APIInstance.h"
#include <string>
#include "EnumMapping.h"
#include "Device.h"
#include "Context.h"
#include "AutoPtr.hpp"
#pragma warning( disable : 4996 )

namespace
{
	const int kMaxPerfNameLength = 256;
	bool GenerateFxoFileToDisk(AutoR<ID3DXBuffer>& pBuffer, const char* path)
	{
		if (!pBuffer)
		{
			return false;
		}
		//输出二进制文件
		char* effectCode = (char*)pBuffer->GetBufferPointer();
		DWORD size = pBuffer->GetBufferSize();
		if (effectCode)
		{
			FILE* pFile = fopen(path, "wb");
			if (pFile)
			{
				/*size_t writesize=*/
				//写fxo文件
				fwrite(effectCode, size, 1, pFile);
				fclose(pFile);
				//LogInfo(ENGINE_INIT, OutPut_File, "GenerateFxoFileToDisk:写fxo文件:%s", path.c_str());
				return true;
			}
			else
			{
				//LogEWithDesc("生成fxo文件失败！fopen failed:%s", path.c_str());
				//针对D3D加载fx失败问题；check  多打印一条
				if (IsLocalFileExist(path))
				{
					//LogInfo(ENGINE_INIT, OutPut_File, "文件\\目录 存在：%s", path.c_str());
				}
				else
				{
					//LogInfo(ENGINE_INIT, OutPut_File, "文件\\目录 不存在：%s", path.c_str());
				}
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

RenderAPI::APIInstance* CreateAPIInstance()
{
	::APIInstance* pAPI = new ::APIInstance();
	if (pAPI->Init())
	{
		return pAPI;
	}
	else
	{
		delete pAPI;
		return NULL;
	}
}

APIInstance::APIInstance()
	: m_hDLL(NULL)
	, m_d3d9ExPtr(NULL)
	, m_d3d9Ptr(NULL)
	, m_supportOcclusionQuery(false)
	, D3DPerfBeginEvent(NULL)
	, D3DPerfEndEvent(NULL)
	, D3DPerfSetMarker(NULL)
{
}

// D3D9EX API CREATION, 手动加载d3d9.dll来获取，以判断是否支持 [1/25/2013 YiKaiming]
// Define a function pointer to the Direct3DCreate9Ex function.
typedef HRESULT(WINAPI *LPDIRECT3DCREATE9EX)(UINT, IDirect3D9Ex**);
typedef IDirect3D9* (WINAPI *LPDIRECT3DCREATE9)(UINT);

bool APIInstance::Init()
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

	CreateD3D();
	if (m_d3d9Ptr == NULL)
	{
		return false;
	}

	D3DADAPTER_IDENTIFIER9 info;
	m_d3d9Ptr->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &info);

	m_deviceDriver = info.Driver;
	m_deviceName = info.DeviceName;
	m_deviceDesc = info.Description;
	m_vendorID = info.VendorId;
	m_driverVersion.Product = HIWORD(info.DriverVersion.HighPart);
	m_driverVersion.Version = LOWORD(info.DriverVersion.HighPart);
	m_driverVersion.Subversion = HIWORD(info.DriverVersion.LowPart);
	m_driverVersion.BuildNumber = LOWORD(info.DriverVersion.LowPart);
	m_driverVersion.WHQLLevel = info.WHQLLevel;
	return true;
}

void APIInstance::Deinit()
{
	DestroyD3D();
	::FreeLibrary(m_hDLL);
}

bool APIInstance::IsSupportD3D9EX() const
{
	return m_d3d9ExPtr != NULL;
}

bool APIInstance::IsSupportOcclusionQuery() const { return m_supportOcclusionQuery; }

bool APIInstance::CheckFormatValidate(D3DFORMAT & renderTarget, D3DFORMAT depthStencil) const
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

bool APIInstance::CheckBackBufferFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const
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

bool APIInstance::CheckDepthStencilFormat(D3DFORMAT checkFormat, D3DFORMAT adapterFormat) const
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

bool APIInstance::CheckDeviceMultiSampleType(D3DFORMAT rtFormat, D3DFORMAT dsFormat, bool isFullsreen, D3DMULTISAMPLE_TYPE mulsampleType) const
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

void APIInstance::CreateD3D()
{
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
}

void APIInstance::DestroyD3D()
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
}

D3DPRESENT_PARAMETERS APIInstance::FillCreationParam(APIInstance& self, HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample)
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

	if (self.CheckDeviceMultiSampleType(rtFormat, dsFormat, isFullscreen, mulsample))
	{
		d3dpp.Flags &= ~D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	}
	else
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	}

	return d3dpp;
}

bool APIInstance::IsSupportManaged()
{
	return !IsSupportD3D9EX();
}

void APIInstance::Recreate()
{
	DestroyD3D();
	CreateD3D();
}

IDirect3DDevice9* APIInstance::CreateDevice(HWND hWindow, unsigned int width, unsigned int height, bool isFullscreen, bool vsync, D3DFORMAT rtFormat, D3DFORMAT dsFormat, D3DMULTISAMPLE_TYPE mulsample)
{
	//by sssa2000 20110120
	DWORD MT = D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE;
	DWORD hardwareBehaviorFlag = D3DCREATE_HARDWARE_VERTEXPROCESSING | MT;
	DWORD softwareBehaviorFlag = D3DCREATE_SOFTWARE_VERTEXPROCESSING | MT;

	D3DPRESENT_PARAMETERS d3dpp = FillCreationParam(*this, hWindow, width, height, isFullscreen, vsync, rtFormat, dsFormat, mulsample);

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

		if (devicePtr != NULL)
		{
			m_supportOcclusionQuery = S_OK == devicePtr->CreateQuery(D3DQUERYTYPE_OCCLUSION, NULL);
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

		if (devicePtr != NULL)
		{
			m_supportOcclusionQuery = S_OK == devicePtr->CreateQuery(D3DQUERYTYPE_OCCLUSION, NULL);
		}
		return devicePtr;
	}
}

void APIInstance::AddRef()
{
	++m_refCount;
}

void APIInstance::LogError(const char* action, const char * detail)
{
	char buff[512];
	_sprintf_p(buff, 512, "%s Failed, %s, ", action, detail);
	GetCurrentLogger()->LogE(buff);
}

void APIInstance::LogError(const char* action, const char * detail, HRESULT errorCode)
{
	char buff[512];
	_sprintf_p(buff, 512, "%s Failed, error:%d, %s, ", action, errorCode, detail);
	GetCurrentLogger()->LogE(buff);
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

	D3DFORMAT rtFormat = s_RTFormats[desc.backbufferFormat];
	D3DFORMAT dsFormat = s_DSFormats[desc.zbufferFormat];
	D3DMULTISAMPLE_TYPE mulsample = s_sampleTypes[desc.aaMode];

	if (CheckFormatValidate(rtFormat, dsFormat))
	{
		IDirect3DDevice9* devicePtr = CreateDevice(
			(HWND)desc.hWindow,
			desc.backbufferWidth, desc.backbufferHeight,
			isFullscreen, useVerticalSync,
			rtFormat, dsFormat,
			mulsample);

		if (devicePtr != NULL)
		{
			RenderAPI::SwapChainDesc newDesc = desc;
			if (desc.backbufferWidth == 0 || desc.backbufferHeight == 0)
			{
				RECT rect;
				::GetClientRect((HWND)desc.hWindow, &rect);
				newDesc.backbufferWidth = rect.right - rect.left;
				newDesc.backbufferHeight = rect.bottom - rect.top;
			}

			result.Success = true;
			hDeviceWindow = (HWND)desc.hWindow;
			
			devicePtr->AddRef();

			result.DevicePtr = new ::Device(this, devicePtr, newDesc, isFullscreen, useVerticalSync);
			
			RenderAPI::SwapChain* swapChain = result.DevicePtr->GetDefaultSwapChain();
			RenderAPI::RenderTarget* rt = swapChain->GetRenderTarget();
			RenderAPI::DepthStencil* ds = swapChain->GetDepthStencil();
			result.ContextPtr = new ::Context(this, devicePtr, rt, ds,m_renderStatistic);
			rt->Release();
			ds->Release();
			swapChain->Release();
		}
	}
	return result;
}

bool APIInstance::CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile, const char* includeDir)
{
	EffectInclude includeCallback(includeDir);
	AutoR<ID3DXBuffer> pErrorBuffer;
	AutoR<ID3DXBuffer> pEffectBuffer;
	ID3DXEffectCompiler* pEffectCompile;
	DWORD flags = D3DXSHADER_USE_LEGACY_D3DX9_31_DLL; //要想支持ps 1_x 需要使用这个。

	HRESULT hr = D3DXCreateEffectCompilerFromFileA(sourceFXFile, NULL, &includeCallback, flags, &pEffectCompile, &pErrorBuffer);
	if (FAILED(hr))
	{
		if (pErrorBuffer.IsNotNullPtr())
		{
			std::string errorStr = (char*)pErrorBuffer->GetBufferPointer();
			LogError("APIInstance::CompileFXEffectFromFile D3DXCreateEffectCompilerFromFileA Failed.", errorStr.c_str(), hr);
		}
		return false;
	}

	//lockobj lock(D3DX_EFFET_LOCK);

	unsigned debugFlag =
#if _DEBUG
		D3DXSHADER_DEBUG;
#else
		0;
#endif

	pErrorBuffer.Release();
	hr = pEffectCompile->CompileEffect(debugFlag, &pEffectBuffer, &pErrorBuffer);
	if (FAILED(hr))
	{
		if (pErrorBuffer.IsNotNullPtr())
		{
			std::string errorStr = (char*)pErrorBuffer->GetBufferPointer();
			LogError("APIInstance::CompileFXEffectFromFile CompileEffect Failed.", errorStr.c_str(), hr);
		}
		return false;
	}

	return GenerateFxoFileToDisk(pEffectBuffer, compiledFXFile);

}

RenderAPI::RenderTargetFormat APIInstance::GetDefaultRenderTargetFormat()
{
	D3DDISPLAYMODE mode;
	m_d3d9Ptr->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

	if (mode.Format == D3DFMT_A8B8G8R8)
		return RenderAPI::RT_ARGB8;
	else
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

struct AAModeMapping
{
	D3DMULTISAMPLE_TYPE d3dAA;
	RenderAPI::AAMode apiAA;
};

bool APIInstance::CheckMultiSampleSupport(RenderAPI::RenderTargetFormat bb, RenderAPI::DepthStencilFormat z, RenderAPI::AAMode aa, bool fullscreen) const
{
	D3DFORMAT rtFormat = s_RTFormats[bb];
	D3DFORMAT dsFormat = s_DSFormats[z];
	D3DMULTISAMPLE_TYPE aaType = s_sampleTypes[aa];
	return CheckDeviceMultiSampleType(rtFormat, dsFormat, fullscreen, aaType);
}

RenderAPI::DriverVersion APIInstance::GetDriverVersion() const
{
	return m_driverVersion;
}

bool APIInstance::IsSupportEx() const { return IsSupportD3D9EX(); }

void APIInstance::PerfBegin(unsigned int color, const char* name)
{
	if (D3DPerfBeginEvent != NULL)
	{
		size_t nameLength = strlen(name);
		size_t outCount = kMaxPerfNameLength;
		wchar_t nameWStr[kMaxPerfNameLength];
		mbstowcs_s(&outCount, nameWStr, name, nameLength);		 
		D3DPerfBeginEvent(color, nameWStr);
	}
}

void APIInstance::PerfMark(unsigned int color, const char * name)
{
	if (D3DPerfSetMarker != NULL)
	{
		size_t nameLength = strlen(name);
		size_t outCount = kMaxPerfNameLength;
		wchar_t nameWStr[kMaxPerfNameLength];
		mbstowcs_s(&outCount, nameWStr, name, nameLength);
		D3DPerfSetMarker(color, nameWStr);
	}
}

void APIInstance::PerfEnd()
{
	if (D3DPerfEndEvent != NULL)
	{
		D3DPerfEndEvent();
	}
}


EffectInclude::EffectInclude(const std::string & includeDir)
	: m_dirInclude(includeDir)
{
	if (!m_dirInclude.empty())
	{
		if (m_dirInclude[m_dirInclude.size() - 1] != '\\' || m_dirInclude[m_dirInclude.size() - 1] != '/')
		{
			m_dirInclude += '\\';
		}
	}
}


std::string GetLocalFileName(const std::string& fullName)
{
	size_t slash = fullName.rfind("\\");
	size_t rslash = fullName.rfind("/");

	if (slash == std::string::npos && rslash == std::string::npos)
		return fullName;
	
	slash = __min(slash, rslash);
	return fullName.substr(slash + 1);
}

HRESULT STDMETHODCALLTYPE EffectInclude::Open(D3DXINCLUDE_TYPE includeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	std::string fileName = pFileName;
	if (!IsLocalFileExist(pFileName))
	{
		bool localInclude = (includeType == D3DXINC_LOCAL) && (pParentData != NULL);
		if (localInclude)
		{
			std::string localFileName = GetLocalFileName(fileName);
			fileName = m_dirInclude + localFileName;

			if (!IsLocalFileExist(fileName))
			{
				//g_Log.OutPutConsole(true,"无法打开Fx Include文件%s.\n当前目录:%s",pFileName,buff);
				return E_INVALIDARG;
			}
		}
		else
		{
			return E_INVALIDARG;
		}

	}

	HANDLE hFile = CreateFileA(fileName.c_str(),		// file to open
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
			//LogEWithDesc("Read Include File 失败%s. pFileName=%s;dest_filename=%s;%d=", pFileName, dest_filename.c_str(), errcode);
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
		//LogEWithDesc("无法打开Fx 文件dest_filename=%s.\n当前目录:%s ;pFileName=%s", dest_filename.c_str(), buff, pFileName);
		return S_FALSE;
	}
}

HRESULT STDMETHODCALLTYPE EffectInclude::Close(LPCVOID pData)
{
	delete[] pData;
	return S_OK;
}



