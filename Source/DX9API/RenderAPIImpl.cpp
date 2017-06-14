#include <stdlib.h>
#include "../../RenderAPI/RenderAPI.h"
#include "Device.h"
#include "Context.h"
#include "RenderAPIImpl.h"
#include "DX9Include.h"

namespace
{
	D3D9DLL s_d3d9Dll;

	const int RTFormatCount = 2;
	D3DFORMAT s_RTFormats[RTFormatCount] =
	{
		D3DFMT_X8R8G8B8,
		D3DFMT_A8R8G8B8,
	};

	const int DSFormatCount = 3;
	D3DFORMAT s_DSFormats[DSFormatCount] =
	{
		D3DFMT_D24S8,
		D3DFMT_D24X8,
		D3DFMT_D16,
	};

	D3DMULTISAMPLE_TYPE s_sampleTypes[] =
	{
		D3DMULTISAMPLE_NONE,
		D3DMULTISAMPLE_2_SAMPLES,
		D3DMULTISAMPLE_4_SAMPLES,
		D3DMULTISAMPLE_6_SAMPLES,
		D3DMULTISAMPLE_8_SAMPLES,
		D3DMULTISAMPLE_10_SAMPLES,
	};
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
				result.Success = true;
				result.DevicePtr = new ::Device(desc, isFullscreen, useVerticalSync);
				result.ContextPtr = new ::Context();

				//��ʱrelease������ȥд����������
				devicePtr->Release();

			}
		}
		return result;
	}
}

RefCount::RefCount() : count(1) { }
