#include <stdlib.h>
#include <io.h>
#include "../../RenderAPI/RenderAPI.h"
#include "Device.h"
#include "Context.h"
#include "EnumMapping.h"
#include "APIContext.h"
#include "DX9Include.h"
#include "AutoPtr.hpp"

D3DFORMAT s_RTFormats[RTFormatCount] =
{
	D3DFMT_X8R8G8B8,
	D3DFMT_A8R8G8B8,
};

D3DFORMAT s_DSFormats[DSFormatCount] =
{
	D3DFMT_D24S8,
	D3DFMT_D24X8,
	D3DFMT_D16,
};

D3DFORMAT s_TextureFormats[TexFormatCount] =
{
	D3DFMT_UNKNOWN,
	D3DFMT_A8R8G8B8,
	D3DFMT_X8R8G8B8,
	D3DFMT_DXT1,
	D3DFMT_DXT3,
	D3DFMT_DXT5,
	D3DFMT_D24S8,
	D3DFMT_D24X8,
	D3DFMT_D32,
	D3DFMT_D16,
};

D3DMULTISAMPLE_TYPE s_sampleTypes[MSFormatCount] =
{
	D3DMULTISAMPLE_NONE,
	D3DMULTISAMPLE_2_SAMPLES,
	D3DMULTISAMPLE_4_SAMPLES,
	D3DMULTISAMPLE_6_SAMPLES,
	D3DMULTISAMPLE_8_SAMPLES,
	D3DMULTISAMPLE_10_SAMPLES,
};

unsigned int s_IndexLengths[IndexLengthCount] =
{
	sizeof(unsigned short),
	sizeof(unsigned int),
};

D3DFORMAT s_IndexFormats[IndexFormatCount] =
{
	D3DFMT_INDEX16,
	D3DFMT_INDEX32,
};

unsigned int s_lockOptions[LockOptionCount] =
{
	0,
	D3DLOCK_DISCARD,
	D3DLOCK_READONLY,
};

bool IsLocalFileExist(const std::string & fileName)
{
	return (_access(fileName.c_str(), 0) != -1);
}

RenderAPI::DeviceState DeviceStateMapping(unsigned int rst)
{
	if (rst == S_OK)
	{
		return RenderAPI::DEVICE_OK;
	}
	else if (rst == D3DERR_DEVICELOST)
	{
		return RenderAPI::DEVICE_Lost;
	}
	else if (rst == D3DERR_DEVICENOTRESET)
	{
		return RenderAPI::DEVICE_NeedReset;
	}
	else if (rst == D3DERR_DRIVERINTERNALERROR)
	{
		// 这个返回值是需要重新创建d3d9对象
		return RenderAPI::DEVICE_NeedRecreate;
	}
	else
	{
		return RenderAPI::DEVICE_Error;
	}

}

namespace
{
	D3D9DLL s_d3d9Dll;

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

namespace RenderAPI
{
	bool Initialize()
	{
		if (!s_d3d9Dll.Init())
		{
			return false;
		}
		return true;
	}

	void Deinitialize()
	{
		s_d3d9Dll.Deinit();
	}

	CreationResult CreateDeviceAndContext(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync)
	{
		CreationResult result;

		D3DFORMAT rtFormat = s_RTFormats[desc.backbufferFormat];
		D3DFORMAT dsFormat = s_DSFormats[desc.zbufferFormat];
		D3DMULTISAMPLE_TYPE mulsample = s_sampleTypes[desc.aaMode];

		if (s_d3d9Dll.CheckFormatValidate(rtFormat, dsFormat))
		{
			IDirect3DDevice9* devicePtr = s_d3d9Dll.CreateDevice(
				(HWND)desc.hWindow,
				desc.backbufferWidth, desc.backbufferHeight,
				isFullscreen, useVerticalSync,
				rtFormat, dsFormat,
				mulsample);

			if (devicePtr != NULL)
			{
				SwapChainDesc newDesc = desc;
				if (desc.backbufferWidth == 0 || desc.backbufferHeight == 0)
				{
					RECT rect;
					::GetClientRect((HWND)desc.hWindow, &rect);
					newDesc.backbufferWidth = rect.right - rect.left;
					newDesc.backbufferHeight = rect.bottom - rect.top;
				}

				result.Success = true;
				LONG count = devicePtr->AddRef();
				APIContext* pAPIContext = new APIContext();
				pAPIContext->AddRef();
				pAPIContext->CreationParam = s_d3d9Dll.MakeCreationParam(
					(HWND)desc.hWindow,
					desc.backbufferWidth, desc.backbufferHeight,
					isFullscreen, useVerticalSync,
					rtFormat, dsFormat,
					mulsample);
				pAPIContext->pD3D = &s_d3d9Dll;
				result.DevicePtr = new ::Device(pAPIContext, devicePtr, newDesc, isFullscreen, useVerticalSync);
				RenderAPI::SwapChain* swapChain = result.DevicePtr->GetDefaultSwapChain();
				RenderAPI::RenderTarget* rt = swapChain->GetRenderTarget();
				RenderAPI::DepthStencil* ds = swapChain->GetDepthStencil();
				result.ContextPtr = new ::Context(pAPIContext, devicePtr, rt, ds);
				rt->Release();
				ds->Release();
				swapChain->Release();
			}
		}
		return result;
	}

	bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile)
	{
		EffectInclude includeCallback;
		AutoR<ID3DXBuffer> pErrorBuffer;
		AutoR<ID3DXBuffer> pEffectBuffer;
		ID3DXEffectCompiler* pEffectCompile;
		DWORD flags = D3DXSHADER_USE_LEGACY_D3DX9_31_DLL; //要想支持ps 1_x 需要使用这个。


		HRESULT hr = D3DXCreateEffectCompilerFromFileA(sourceFXFile, NULL, &includeCallback, flags, &pEffectCompile, &pErrorBuffer);
		if (FAILED(hr))
		{
			//PrintFxError("FX编译失败！D3DXCreateEffectCompilerFromFile Failed", pErrorBuffer);
			return false;
		}

		//lockobj lock(D3DX_EFFET_LOCK);

		unsigned debugFlag =
#if _DEBUG
			D3DXSHADER_DEBUG;
#else
			0;
#endif
		hr = pEffectCompile->CompileEffect(debugFlag, &pEffectBuffer, &pErrorBuffer);
		if (FAILED(hr))
		{
			//PrintFxError("FX编译失败！CompileEffect Failed", pErrorBuffer);
			return false;
		}

		return GenerateFxoFileToDisk(pEffectBuffer, compiledFXFile);
	}
}


