#include <stdlib.h>
#include "Device.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "FXEffect.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "APIContext.h"

namespace
{
	const int s_d3dUsageCount = 3;
	unsigned int s_d3dUsage[s_d3dUsageCount] =
	{
		D3DUSAGE_WRITEONLY,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		D3DUSAGE_WRITEONLY,
	};
}

Device::Device(APIContext* pAPIContext, IDirect3DDevice9* device, const RenderAPI::SwapChainDesc & desc, bool isFullscreen, bool useVerticalSync)
	: m_pAPIContext(pAPIContext)
	, m_pDefaultSwapChain(NULL)
	, m_pDevice(device)
{
	m_pAPIContext->pDevice = this;
	IDirect3DSwapChain9* pSwapChain = NULL;
	m_pDevice->GetSwapChain(0, &pSwapChain);
	IDirect3DSurface9* pDSSurafce = NULL;
	m_pDevice->GetDepthStencilSurface(&pDSSurafce);
	::DepthStencil* pDepthStencil = new DepthStencil(pDSSurafce, desc.zbufferFormat, desc.backbufferWidth, desc.backbufferHeight);
	m_pDefaultSwapChain = new ::SwapChain(pSwapChain, pDepthStencil, desc);
}

Device::~Device()
{
	m_pDefaultSwapChain->Release();
	m_pDevice->Release();
	m_pAPIContext->Release();
	m_pDefaultSwapChain = NULL;
	m_pDevice = NULL;
	m_pAPIContext = NULL;
}

RenderAPI::SwapChain * Device::GetDefaultSwapChain()
{
	m_pDefaultSwapChain->AddRef();
	return m_pDefaultSwapChain;
}

RenderAPI::SwapChain * Device::CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc & swapChainDesc)
{
	IDirect3DSwapChain9* swapChain = NULL;
	D3DPRESENT_PARAMETERS param;
	ZeroMemory(&param, sizeof(param));

	D3DFORMAT rtFormat = s_RTFormats[swapChainDesc.backbufferFormat];
	D3DFORMAT dsFormat = s_DSFormats[swapChainDesc.zbufferFormat];
	D3DMULTISAMPLE_TYPE mulsample = s_sampleTypes[swapChainDesc.aaMode];

	param.BackBufferWidth = swapChainDesc.backbufferWidth;
	param.BackBufferHeight = swapChainDesc.backbufferHeight;
	param.BackBufferFormat = rtFormat;
	param.BackBufferCount = 1;

	param.MultiSampleType = mulsample;
	param.MultiSampleQuality = 0;

	param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	param.hDeviceWindow = (HWND)swapChainDesc.hWindow;
	param.Windowed = TRUE;
	param.EnableAutoDepthStencil = TRUE;
	param.AutoDepthStencilFormat = dsFormat;

	param.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr = m_pDevice->CreateAdditionalSwapChain(&param, &swapChain);
	if (hr == S_OK)
	{
		RenderAPI::SwapChainDesc newDesc = swapChainDesc;
		if (swapChainDesc.backbufferWidth == 0 || swapChainDesc.backbufferHeight == 0)
		{
			RECT rect;
			::GetClientRect((HWND)swapChainDesc.hWindow, &rect);
			newDesc.backbufferWidth = rect.right - rect.left;
			newDesc.backbufferHeight = rect.bottom - rect.top;
		}

		::DepthStencil* ds = CreateDepthStencilImplement(newDesc.zbufferFormat, newDesc.backbufferWidth, newDesc.backbufferHeight);
		if (ds == NULL)
		{
			swapChain->Release();
			return NULL;
		}
		else
		{
			return new ::SwapChain(swapChain, ds, newDesc);
		}
	}
	else
	{
		return NULL;
	}
}

RenderAPI::VertexBuffer* Device::CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::VertexElement * elements, unsigned int elementCount, void * initialData)
{
	if (vertexCount == 0 || vertexSize == 0 || elementCount == 0 || elements == NULL)
	{
		return NULL;
	}

	bool immuable = usage == RenderAPI::RESUSAGE_Immuable;
	if (initialData == NULL && immuable)
	{
		return NULL;
	}

	IDirect3DVertexBuffer9* pVertexBuffer = NULL;
	unsigned int d3dUsage = s_d3dUsage[usage];
	unsigned int bufferSize = vertexCount * vertexSize;

	D3DPOOL pool = (m_pAPIContext->pD3D->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateVertexBuffer(bufferSize, d3dUsage, 0, pool, &pVertexBuffer, NULL);
	if (hr != S_OK)
	{
		return NULL;
	}

	if (initialData != NULL)
	{
		void* dataPtr = NULL;
		if (S_OK == pVertexBuffer->Lock(0, bufferSize, &dataPtr, D3DLOCK_DISCARD))
		{
			memcpy(dataPtr, initialData, bufferSize);
			pVertexBuffer->Unlock();
		}
	}

	return new VertexBuffer(pVertexBuffer, usage, vertexCount, vertexSize, elements, elementCount);

}

RenderAPI::IndexBuffer* Device::CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount, void * initialData)
{
	if (indexCount == 0)
	{
		return NULL;
	}

	bool immuable = usage == RenderAPI::RESUSAGE_Immuable;
	if (initialData == NULL && immuable)
	{
		return NULL;
	}

	IDirect3DIndexBuffer9* pIndexBuffer = NULL;
	unsigned int d3dUsage = s_d3dUsage[usage];
	unsigned int bufferSize = indexCount * s_IndexLengths[format];
	D3DPOOL pool = (m_pAPIContext->pD3D->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateIndexBuffer(bufferSize, d3dUsage, s_IndexFormats[format], pool, &pIndexBuffer, NULL);

	if (hr != S_OK)
	{
		return NULL;
	}

	if (initialData != NULL)
	{
		void* dataPtr = NULL;
		if (S_OK == pIndexBuffer->Lock(0, bufferSize, &dataPtr, D3DLOCK_DISCARD))
		{
			memcpy(dataPtr, initialData, bufferSize);
			pIndexBuffer->Unlock();
		}
	}

	return new IndexBuffer(pIndexBuffer, usage, format, indexCount);
}

RenderAPI::Texture2D * Device::CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, void* initialData, int dataLinePitch, int dataHeight)
{
	if (width == 0 || height == 0)
	{
		return NULL;
	}

	bool immuable = usage == RenderAPI::RESUSAGE_Immuable;
	if (initialData == NULL)
	{
		if (immuable)
		{
			return NULL;
		}
	}
	else
	{
		if (dataLinePitch == 0 || dataHeight == 0)
		{
			return NULL;
		}
	}


	IDirect3DTexture9* pTexture = NULL;
	unsigned int d3dUsage = s_d3dUsage[usage];
	D3DPOOL pool = (m_pAPIContext->pD3D->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateTexture(width, height, 0, d3dUsage, s_TextureFormats[format], pool, &pTexture, NULL);

	if (hr != S_OK)
	{
		return NULL;
	}

	if (initialData != NULL)
	{
		D3DLOCKED_RECT rect;
		if (S_OK == pTexture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD))
		{
			int linePitch = dataLinePitch > rect.Pitch ? rect.Pitch : dataLinePitch;
			char* dstPtr = (char*)rect.pBits;
			char* srcPtr = (char*)initialData;
			for (int i = 0; i < dataHeight; i++)
			{
				memcpy(dstPtr + i * rect.Pitch, srcPtr + i* dataLinePitch, linePitch);
			}
			pTexture->UnlockRect(0);
		}
	}

	return new Texture2D(pTexture, format, usage, width, height);
}

RenderAPI::FXEffect * Device::CreateFXEffectFromFile(const char * effectFilePath)
{
	ID3DXEffect* pEffect = NULL;
	AutoR<ID3DXBuffer> pErrorBuffer;
	EffectInclude includeCallback;
	DWORD flags = D3DXSHADER_USE_LEGACY_D3DX9_31_DLL; //要想支持ps 1_x 需要使用这个。
	 

	HRESULT hr = D3DXCreateEffectFromFileA(m_pDevice, effectFilePath, 0, &includeCallback, flags, 0, &pEffect, &pErrorBuffer);
	if (SUCCEEDED(hr))
	{
		//CacheNames();
		//pEffect->SetStateManager(pDeviceD3D->GetStateManager());
		//读fx文件
		//LogInfo(ENGINE_INIT, OutPut_File, "LoadFxFromDisk:读fxo文件:%s", fxofilename.c_str());
		pEffect->SetStateManager(m_pAPIContext->pContext->GetStateManager());
		return new FXEffect(pEffect);
	}
	else
	{
		// LogError("从磁盘加载fx失败: %s", fxofilename.c_str());
		//PrintErrorIinfo(pErrorBuffer);
		std::string errorStr = (char*)pErrorBuffer->GetBufferPointer();
		return NULL;
	}

}

RenderAPI::RenderTarget * Device::CreateRenderTarget(RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
{
	D3DFORMAT rtFormat = s_RTFormats[format];
	IDirect3DTexture9* pTexture = NULL;
	HRESULT hr = m_pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, rtFormat, D3DPOOL_DEFAULT, &pTexture, NULL);

	if (hr != S_OK)
	{
		return NULL;
	}

	return new RenderTarget(pTexture, format, width, height);
}

RenderAPI::DepthStencil * Device::CreateDepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height)
{
	return CreateDepthStencilImplement(format, width, height);
}

::DepthStencil* Device::CreateDepthStencilImplement(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height)
{
	IDirect3DSurface9* pDSSurface = NULL;
	HRESULT hr = m_pDevice->CreateDepthStencilSurface(width, height, s_DSFormats[format], D3DMULTISAMPLE_NONE, 0, TRUE, &pDSSurface, NULL);
	if (hr != S_OK)
	{
		return NULL;
	}

	return new DepthStencil(pDSSurface, format, width, height);
}

void Device::Release()
{
	delete this;
}