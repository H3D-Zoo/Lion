#include <stdlib.h>
#include <io.h>
#include "../../RenderAPI/RenderAPI.h"
#include "Device.h"
#include "Context.h"
#include "RenderAPIImpl.h"
#include "APIContext.h"
#include "DX9Include.h"

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

D3DFORMAT s_TextureFormats[TexFormatCount]
{
	D3DFMT_UNKNOWN,
	D3DFMT_A8R8G8B8,
	D3DFMT_A8B8G8R8,
	D3DFMT_X8R8G8B8,
	D3DFMT_X8B8G8R8,
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

unsigned int s_lockOptions[LockOptionCount]
{
	0,
	D3DLOCK_DISCARD,
	D3DLOCK_READONLY,
};

bool IsLocalFileExist(const std::string & fileName)
{
	return (_access(fileName.c_str(), 0) != -1);
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
		//����������ļ�
		char* effectCode = (char*)pBuffer->GetBufferPointer();
		DWORD size = pBuffer->GetBufferSize();
		if (effectCode)
		{
			FILE* pFile = fopen(path, "wb");
			if (pFile)
			{
				/*size_t writesize=*/
				//дfxo�ļ�
				fwrite(effectCode, size, 1, pFile);
				fclose(pFile);
				//LogInfo(ENGINE_INIT, OutPut_File, "GenerateFxoFileToDisk:дfxo�ļ�:%s", path.c_str());
				return true;
			}
			else
			{
				//LogErrorWithDesc("����fxo�ļ�ʧ�ܣ�fopen failed:%s", path.c_str());
				//���D3D����fxʧ�����⣻check  ���ӡһ��
				if (IsLocalFileExist(path))
				{
					//LogInfo(ENGINE_INIT, OutPut_File, "�ļ�\\Ŀ¼ ���ڣ�%s", path.c_str());
				}
				else
				{
					//LogInfo(ENGINE_INIT, OutPut_File, "�ļ�\\Ŀ¼ �����ڣ�%s", path.c_str());
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
		AutoR<ID3DXBuffer> pErrorBuffer = NULL;
		AutoR<ID3DXBuffer> pEffectBuffer = NULL;
		ID3DXEffectCompiler* pEffectCompile;
		DWORD flags = D3DXSHADER_USE_LEGACY_D3DX9_31_DLL; //Ҫ��֧��ps 1_x ��Ҫʹ�������


		HRESULT hr = D3DXCreateEffectCompilerFromFileA(sourceFXFile, NULL, &includeCallback, flags, &pEffectCompile, &pErrorBuffer);
		if (FAILED(hr))
		{
			//PrintFxError("FX����ʧ�ܣ�D3DXCreateEffectCompilerFromFile Failed", pErrorBuffer);
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
			//PrintFxError("FX����ʧ�ܣ�CompileEffect Failed", pErrorBuffer);
			return false;
		}

		return GenerateFxoFileToDisk(pEffectBuffer, compiledFXFile);
	}
}

RefCount::RefCount() : count(1) { }
