#include <stdlib.h>
#include "Device.h"
#include "Context.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "FXEffect.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "AutoPtr.hpp"
#include "EnumMapping.h"
#include "OcclusionQuery.h"

namespace
{
	const int kD3DUsageCount = 5;
	unsigned int s_d3dBufferUsage[kD3DUsageCount] =
	{
		D3DUSAGE_WRITEONLY,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		D3DUSAGE_WRITEONLY,
		0,
		D3DUSAGE_DYNAMIC
	};
}

Device::Device(APIInstance* pAPIContext, IDirect3DDevice9* device, const RenderAPI::SwapChainDesc & desc, bool isFullscreen, bool useVerticalSync)
	: m_pAPI(pAPIContext)
	, m_pDefaultSwapChain(NULL)
	, m_pDevice(device)
{
	m_pAPI->pDevice = this;
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
	m_pAPI->Release();
	m_pDefaultSwapChain = NULL;
	m_pDevice = NULL;
	m_pAPI = NULL;
}

void Device::ReleaseDefaultSwapChainWhenLost()
{
	m_pDefaultSwapChain->ReleaseSurfaceWhenLost();
}

void Device::ResetDefaultBackBuffer(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, RenderAPI::DepthStencilFormat dsFormat)
{
	IDirect3DSurface9* pDSSurafce = NULL;
	m_pDevice->GetDepthStencilSurface(&pDSSurafce);
	m_pDefaultSwapChain->ResetBackBuffers(width, height, rtFormat, dsFormat, pDSSurafce);
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

RenderAPI::VertexBuffer* Device::CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, void * initialData)
{
	if (vertexCount == 0 || vertexSize == 0)
	{
		return NULL;
	}

	bool immuable = usage == RenderAPI::RESUSAGE_Immuable;
	if (initialData == NULL && immuable)
	{
		return NULL;
	}

	IDirect3DVertexBuffer9* pVertexBuffer = NULL;
	unsigned int d3dUsage = s_d3dBufferUsage[usage];
	unsigned int bufferSize = vertexCount * vertexSize;

	D3DPOOL pool = (m_pAPI->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
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

	return new VertexBuffer(pVertexBuffer, usage, vertexCount, vertexSize, pool != D3DPOOL_MANAGED);

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
	unsigned int d3dUsage = s_d3dBufferUsage[usage];
	unsigned int bufferSize = indexCount * s_IndexLengths[format];
	D3DPOOL pool = (m_pAPI->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
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

	return new IndexBuffer(pIndexBuffer, usage, format, indexCount, pool != D3DPOOL_MANAGED);
}

namespace
{
	struct DeclFormat
	{
		D3DDECLTYPE Type;
		int Length;
	};

	DeclFormat s_declTypes[] =
	{
		{ D3DDECLTYPE_FLOAT1, sizeof(float) },
		{ D3DDECLTYPE_FLOAT2, sizeof(float) * 2 },
		{ D3DDECLTYPE_FLOAT3, sizeof(float) * 3 },
		{ D3DDECLTYPE_FLOAT4, sizeof(float) * 4 },
		{ D3DDECLTYPE_D3DCOLOR, sizeof(unsigned char) * 4 },
		{ D3DDECLTYPE_UBYTE4, sizeof(unsigned char) * 4 },
		{ D3DDECLTYPE_SHORT2, sizeof(short) * 2 },
		{ D3DDECLTYPE_SHORT4, sizeof(short) * 4 },
		{ D3DDECLTYPE_UBYTE4N, sizeof(unsigned char) * 4 },
		{ D3DDECLTYPE_SHORT2N, sizeof(short) * 2 },
		{ D3DDECLTYPE_SHORT4N, sizeof(short) * 4 },
		{ D3DDECLTYPE_USHORT2N, sizeof(unsigned short) * 2 },
		{ D3DDECLTYPE_USHORT4N, sizeof(unsigned short) * 4 },
	};

	D3DDECLUSAGE s_declUsages[] =
	{
		D3DDECLUSAGE_POSITION,
		D3DDECLUSAGE_COLOR,
		D3DDECLUSAGE_NORMAL,
		D3DDECLUSAGE_TEXCOORD,
		D3DDECLUSAGE_TANGENT,
		D3DDECLUSAGE_BINORMAL,
		D3DDECLUSAGE_BLENDWEIGHT,
		D3DDECLUSAGE_BLENDINDICES
	};
}
RenderAPI::VertexDeclaration* Device::CreateVertexDeclaration(const RenderAPI::VertexElement * elements, unsigned int elementCount)
{
	if (elementCount == 0)
	{
		return NULL;
	}

	//构建 Vertex Element 数组
	std::vector<int> alignOffsets;
	std::vector<RenderAPI::VertexElement> elementList(elements, elements + elementCount);

	std::vector<D3DVERTEXELEMENT9> d3dElements(elementCount + 1);
	for (unsigned int i = 0; i < elementCount; i++)
	{
		D3DVERTEXELEMENT9& element = d3dElements[i];
		RenderAPI::VertexElement& ve = elementList[i];
		DeclFormat& fmt = s_declTypes[ve.Format];

		element.Stream = ve.StreamIndex;
		while (alignOffsets.size() <= ve.StreamIndex)
		{
			alignOffsets.push_back(0);
		}
		int& offset = alignOffsets[ve.StreamIndex];
		element.Offset = ve.AlignOffset = ve.AlignOffset == 0xFFFFFFFF ? offset : ve.AlignOffset;
		element.Type = fmt.Type;
		element.Method = D3DDECLMETHOD_DEFAULT;
		element.Usage = s_declUsages[ve.SemanticName];
		element.UsageIndex = ve.SemanticIndex;

		offset += fmt.Length;
	}

	D3DVERTEXELEMENT9& elementEnd = d3dElements[elementCount];
	elementEnd.Stream = 0xFF;
	elementEnd.Offset = 0;
	elementEnd.Type = D3DDECLTYPE_UNUSED;
	elementEnd.Method = 0;
	elementEnd.Usage = 0;
	elementEnd.UsageIndex = 0;

	IDirect3DVertexDeclaration9* pVertexDeclaration = NULL;
	if (S_OK == m_pDevice->CreateVertexDeclaration(&(d3dElements[0]), &pVertexDeclaration))
	{
		return new VertexDeclaration(pVertexDeclaration, elementList);
	}
	else
	{
		return NULL;
	}
}

RenderAPI::Texture2D * Device::CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps, void* initialData, int dataLinePitch, int dataHeight)
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

	if (autoGenMipmaps && layer == 1)
	{
		layer = 0;
	}
	else if (layer == 0 && !autoGenMipmaps)
	{
		autoGenMipmaps = true;
	}

	IDirect3DTexture9* pTexture = NULL;
	unsigned int d3dUsage = autoGenMipmaps ? D3DUSAGE_AUTOGENMIPMAP : 0;
	if (usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicRW)
		d3dUsage |= D3DUSAGE_DYNAMIC;
	D3DPOOL pool = (m_pAPI->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateTexture(width, height, layer, d3dUsage, s_TextureFormats[format], pool, &pTexture, NULL);

	if (hr != S_OK)
	{
		return NULL;
	}

	::Texture2D* texture = new ::Texture2D(pTexture, format, usage, width, height, autoGenMipmaps, pool != D3DPOOL_MANAGED, false);

	if (initialData != NULL)
	{
		RenderAPI::MappedResource res = texture->LockRect(0, RenderAPI::LOCK_Discard);

		if (res.Success)
		{
			unsigned int linePitch = (unsigned int)dataLinePitch > res.LinePitch ? res.LinePitch : dataLinePitch;
			char* dstPtr = (char*)res.DataPtr;
			char* srcPtr = (char*)initialData;
			for (int i = 0; i < dataHeight; i++)
			{
				memcpy(dstPtr + i * res.LinePitch, srcPtr + i* dataLinePitch, linePitch);
			}
			texture->UnlockRect(0);
		}
	}
	return texture;
}

RenderAPI::TextureCube * Device::CreateTextureCube(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int edgeLength, unsigned int layer, bool autoGenMipmaps, void ** initialData, int dataLinePitch, int dataHeight)
{
	if (edgeLength == 0)
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

	if (autoGenMipmaps && layer == 1)
	{
		layer = 0;
	}
	else if (layer == 0 && !autoGenMipmaps)
	{
		autoGenMipmaps = true;
	}

	IDirect3DCubeTexture9* pTexture = NULL;
	unsigned int d3dUsage = autoGenMipmaps ? D3DUSAGE_AUTOGENMIPMAP : 0;
	if (usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicRW)
		d3dUsage |= D3DUSAGE_DYNAMIC;
	D3DPOOL pool = (m_pAPI->IsSupportManaged() && immuable) ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateCubeTexture(edgeLength, layer, d3dUsage, s_TextureFormats[format], pool, &pTexture, NULL);

	if (hr != S_OK)
	{
		return NULL;
	}

	::TextureCube* texture = new ::TextureCube(pTexture, format, usage, edgeLength, autoGenMipmaps, pool != D3DPOOL_MANAGED);
	if (initialData != NULL)
	{
		RenderAPI::CubemapFace faces[6] = {
			RenderAPI::CUBE_PositiveZ,
			RenderAPI::CUBE_NegativeZ,
			RenderAPI::CUBE_NegativeX,
			RenderAPI::CUBE_PositiveX,
			RenderAPI::CUBE_NegativeY,
			RenderAPI::CUBE_PositiveY
		};
		
		for( int i = 0; i < 6; i++)
		{
			RenderAPI::CubemapFace face = faces[i];
			RenderAPI::MappedResource res = texture->LockRect(face, 0, RenderAPI::LOCK_Discard);
			char* pSrcData = (char*)initialData[i];

			if (res.Success)
			{
				unsigned int linePitch = (unsigned int)dataLinePitch > res.LinePitch ? res.LinePitch : dataLinePitch;
				char* dstPtr = (char*)res.DataPtr;
				char* srcPtr = pSrcData;
				for (int i = 0; i < dataHeight; i++)
				{
					memcpy(dstPtr + i * res.LinePitch, srcPtr + i* dataLinePitch, linePitch);
				}
				texture->UnlockRect(face, 0);
			}
		}
	}
	return texture;
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
		pEffect->SetStateManager(m_pAPI->pContext->GetStateManager());
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

RenderAPI::RenderTarget* Device::CreateRenderTarget(RenderAPI::TextureFormat format, unsigned int width, unsigned int height)
{
	D3DFORMAT rtFormat = s_TextureFormats[format];
	IDirect3DTexture9* pTexture = NULL;
	HRESULT hr = m_pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, rtFormat, D3DPOOL_DEFAULT, &pTexture, NULL);

	if (hr != S_OK)
	{
		return NULL;
	}

	return new RenderTarget(pTexture, format, width, height);
}

RenderAPI::DepthStencil * Device::CreateDepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
{
	return CreateDepthStencilImplement(format, width, height);
}

RenderAPI::OcclusionQuery* Device::CreateOcclusionQuery()
{
	if (m_pAPI->IsSupportOcclusionQuery())
	{
		IDirect3DQuery9* pOcclusionQuery = NULL;
		if (S_OK == m_pDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &pOcclusionQuery))
		{
			return new OcclusionQuery(pOcclusionQuery);
		}
	}
	return NULL;
}

void* Device::GetImplementPtr()
{
	return m_pDevice;
}

::DepthStencil* Device::CreateDepthStencilImplement(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
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