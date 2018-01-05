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
#include "TextBox.h"

namespace
{
	const int kD3DUsageCount = 6;

	unsigned int s_d3dBufferUsage[kD3DUsageCount] =
	{
		D3DUSAGE_WRITEONLY,
		0,
		D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
		0,
		0,
		D3DUSAGE_DYNAMIC,
	};

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

	RenderAPI::TextureFormat GetRenderAPIFormat(D3DFORMAT fmt)
	{
		switch (fmt)
		{
		default:					return RenderAPI::TEX_Unknown;
		case D3DFMT_A8R8G8B8:		return RenderAPI::TEX_ARGB;
		case D3DFMT_X8R8G8B8:		return RenderAPI::TEX_XRGB;
		case D3DFMT_DXT1:			return RenderAPI::TEX_DXT1;
		case D3DFMT_DXT3:			return RenderAPI::TEX_DXT3;
		case D3DFMT_DXT5:			return RenderAPI::TEX_DXT5;
		case D3DFMT_D24S8:			return RenderAPI::TEX_D24S8;
		case D3DFMT_D24X8:			return RenderAPI::TEX_D24X8;
		case D3DFMT_D32:			return RenderAPI::TEX_D32;
		case D3DFMT_D16:			return RenderAPI::TEX_D16;
		case D3DFMT_R32F:			return RenderAPI::TEX_R32F;
		case D3DFMT_G32R32F:		return RenderAPI::TEX_RG32F;
		case D3DFMT_A32B32G32R32F:	return RenderAPI::TEX_ARGB32F;
		};
	}
}

Device::Device(APIInstance* pAPI, IDirect3DDevice9* device, const RenderAPI::SwapChainDesc & desc, bool isFullscreen, bool useVerticalSync)
	: m_pAPIInstance(pAPI)
	, m_pDefaultSwapChain(NULL)
	, m_pDevice(device)
{
	m_pAPIInstance->AddRef();

	IDirect3DSwapChain9* pSwapChain = NULL;
	m_pDevice->GetSwapChain(0, &pSwapChain);
	IDirect3DSurface9* pDSSurafce = NULL;
	m_pDevice->GetDepthStencilSurface(&pDSSurafce);
	::DepthStencil* pDepthStencil = new DepthStencil(pDSSurafce, desc.zbufferFormat, desc.backbufferWidth, desc.backbufferHeight);
	m_pDefaultSwapChain = new ::SwapChain(pSwapChain, pDepthStencil, desc, *m_pAPIInstance);

	D3DCAPS9 d3dcaps;
	m_pDevice->GetDeviceCaps(&d3dcaps);
	m_supportDynamicTexture = (d3dcaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) != 0;
}

Device::~Device()
{
	m_pDefaultSwapChain->Release();
	m_pDevice->Release();
	m_pAPIInstance->Release();
	m_pDefaultSwapChain = NULL;
	m_pDevice = NULL;
	m_pAPIInstance = NULL;
}

void Device::ReleaseDefaultSwapChainWhenLost()
{
	LOG_FUNCTION_CALL(*m_pAPIInstance);

	m_pDefaultSwapChain->ReleaseSurfaceWhenLost();
}

void Device::ResetDefaultBackBuffer(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, RenderAPI::DepthStencilFormat dsFormat)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "width=%d, height=%d, rt format=%d, ds format=%d", width, height, rtFormat, dsFormat);

	IDirect3DSurface9* pDSSurafce = NULL;
	m_pDevice->GetDepthStencilSurface(&pDSSurafce);
	m_pDefaultSwapChain->ResetBackBuffers(width, height, rtFormat, dsFormat, pDSSurafce);
}

RenderAPI::SwapChain * Device::GetDefaultSwapChain()
{
	LOG_FUNCTION_CALL(*m_pAPIInstance);

	m_pDefaultSwapChain->AddReference();
	return m_pDefaultSwapChain;
}

RenderAPI::SwapChain * Device::CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc & swapChainDesc)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "hwindow=%X, width=%d, height=%d, rt format=%d, ds format=%d, aamode = %d", 
		swapChainDesc.hWindow,
		swapChainDesc.backbufferWidth, 
		swapChainDesc.backbufferHeight,
		swapChainDesc.backbufferFormat,
		swapChainDesc.zbufferFormat,
		swapChainDesc.aaMode);

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
			return new ::SwapChain(swapChain, ds, newDesc, *m_pAPIInstance);
		}
	}
	else
	{
		return NULL;
	}
}

void FormatBufferUsage(RenderAPI::ResourceUsage& usage, bool& dynamic, bool& managed)
{
	if (managed)
	{
		if (usage == RenderAPI::RESUSAGE_StaticWOManaged)
		{
			usage = RenderAPI::RESUSAGE_StaticManaged;
			dynamic = false;
		}
		else
		{
			managed = usage == RenderAPI::RESUSAGE_DynamicManaged ||
				usage == RenderAPI::RESUSAGE_StaticManaged;

			dynamic = usage == RenderAPI::RESUSAGE_Dynamic ||
				usage == RenderAPI::RESUSAGE_DynamicManaged;
		}
	}
	else
	{
		if (usage == RenderAPI::RESUSAGE_DynamicManaged)
		{
			usage = RenderAPI::RESUSAGE_Dynamic;
			dynamic = true;
		}
		else if (usage == RenderAPI::RESUSAGE_StaticManaged)
		{
			usage = RenderAPI::RESUSAGE_Static;
			dynamic = false;
		}
		else if (usage == RenderAPI::RESUSAGE_StaticWOManaged)
		{
			usage = RenderAPI::RESUSAGE_StaticWO;
			dynamic = false;
		}
		else
		{
			dynamic = usage == RenderAPI::RESUSAGE_Dynamic;
		}

	}
}

RenderAPI::VertexBuffer* Device::CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "usage=%d, vertex count=%d, vertex stride=%d",
		usage, vertexCount, vertexSize);

	if (vertexCount == 0 || vertexSize == 0)
	{
		LOG_FUNCTION_FAILED(m_pAPIInstance, "Vertex Count and Vertex Size cannot be 0.");
		return NULL;
	}

	bool managed = m_pAPIInstance->IsSupportManaged();
	bool dynamic = false;
	FormatBufferUsage(usage, dynamic, managed);

	IDirect3DVertexBuffer9* pVertexBuffer = NULL;
	unsigned int d3dUsage = s_d3dBufferUsage[usage];
	unsigned int bufferSize = vertexCount * vertexSize;
	D3DPOOL d3dPool = managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateVertexBuffer(bufferSize, d3dUsage, 0, d3dPool, &pVertexBuffer, NULL);
	if (hr != S_OK)
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateVertexBuffer Failed", hr);
		return NULL;
	}
	return new VertexBuffer(pVertexBuffer, usage, managed, vertexCount, vertexSize, *m_pAPIInstance);

}

RenderAPI::IndexBuffer* Device::CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "usage=%d, index format=%s, index count=%d",
		usage, (format == RenderAPI::INDEX_Int16 ? "index16":"index32"), indexCount);

	if (indexCount == 0)
	{
		LOG_FUNCTION_FAILED(m_pAPIInstance, "Index Count cannot be 0.");
		return NULL;
	}


	bool managed = m_pAPIInstance->IsSupportManaged();
	bool dynamic = false;
	FormatBufferUsage(usage, dynamic, managed);

	IDirect3DIndexBuffer9* pIndexBuffer = NULL;
	unsigned int d3dUsage = s_d3dBufferUsage[usage];
	unsigned int bufferSize = indexCount * s_IndexLengths[format];
	D3DPOOL d3dPool = managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateIndexBuffer(bufferSize, d3dUsage, s_IndexFormats[format], d3dPool, &pIndexBuffer, NULL);

	if (hr != S_OK)
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateIndexBuffer Failed", hr);
		return NULL;
	}

	return new IndexBuffer(pIndexBuffer, usage, format, managed, indexCount, *m_pAPIInstance);
}

RenderAPI::VertexDeclaration* Device::CreateVertexDeclaration(const RenderAPI::VertexElement * elements, unsigned int elementCount)
{
	LOG_FUNCTION_CALL(*m_pAPIInstance);

	if (elementCount == 0 || elements == NULL)
	{
		LOG_FUNCTION_FAILED_INVALID_CALL(m_pAPIInstance, "Element Count cannot be 0.");
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
	HRESULT hr = m_pDevice->CreateVertexDeclaration(&(d3dElements[0]), &pVertexDeclaration);
	if (S_OK == hr)
	{
		return new VertexDeclaration(pVertexDeclaration, elementList);
	}
	else
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateVertexDeclaration Failed", hr);
		return NULL;
	}
}

void FormatTextureUsage(RenderAPI::ResourceUsage& usage, bool& dynamic, bool& managed)
{
	if (dynamic)
	{
		dynamic = usage == RenderAPI::RESUSAGE_DynamicManaged || usage == RenderAPI::RESUSAGE_Dynamic;
	}
	else
	{
		if (usage == RenderAPI::RESUSAGE_DynamicManaged)
		{
			usage = RenderAPI::RESUSAGE_StaticManaged;
		}
		else if (usage == RenderAPI::RESUSAGE_Dynamic)
		{
			usage = RenderAPI::RESUSAGE_Static;
		}
	}

	if (managed)
	{
		if (usage == RenderAPI::RESUSAGE_StaticWOManaged)
		{
			usage = RenderAPI::RESUSAGE_StaticManaged;
		}
		else
		{
			managed = usage == RenderAPI::RESUSAGE_DynamicManaged || usage == RenderAPI::RESUSAGE_StaticManaged;
		}
	}
	else
	{
		if (usage == RenderAPI::RESUSAGE_DynamicManaged)
		{
			usage = RenderAPI::RESUSAGE_Dynamic;
		}
		else if (usage == RenderAPI::RESUSAGE_StaticManaged ||
			usage == RenderAPI::RESUSAGE_StaticWOManaged ||
			usage == RenderAPI::RESUSAGE_StaticWO)
		{
			usage = RenderAPI::RESUSAGE_Static;
		}
	}
}

RenderAPI::Texture2D * Device::CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "usage=%d, format=%d, width=%d, height=%d, layer=%d, auto generate mipmaps=%d",
		usage, format, width, height, layer, autoGenMipmaps);

	if (width == 0 || height == 0)
	{
		LOG_FUNCTION_FAILED_INVALID_CALL(m_pAPIInstance, "Width and Height cannot be 0.");
		return NULL;
	}

	if (layer == 1)
	{
		autoGenMipmaps = false;
	}

	bool managed = m_pAPIInstance->IsSupportManaged();
	bool dynamic = m_supportDynamicTexture;
	FormatTextureUsage(usage, dynamic, managed);

	IDirect3DTexture9* pTexture = NULL;
	unsigned int d3dUsage = autoGenMipmaps ? D3DUSAGE_AUTOGENMIPMAP : 0;
	if (dynamic) d3dUsage |= D3DUSAGE_DYNAMIC;
	D3DPOOL d3dPool = managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateTexture(width, height, layer, d3dUsage, s_TextureFormats[format], d3dPool, &pTexture, NULL);
	if (hr != S_OK)
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateTexture Failed.", hr);
		return NULL;
	}

	::Texture2D* texture = NULL;

	//Textures created with D3DPOOL_DEFAULT are not lockable.
	//Textures created in video memory are lockable when created with USAGE_DYNAMIC.
	//D3DLOCK_DISCARD, is only valid when the resource is created with USAGE_DYNAMIC.
	if (dynamic || managed)
	{
		texture = new ::Texture2D(pTexture, format, usage, managed, width, height, autoGenMipmaps, *m_pAPIInstance);
	}
	else
	{
		texture = new ::NoLockableTexture2D(pTexture, format, usage, managed, width, height, autoGenMipmaps, *m_pAPIInstance);
	}

	return texture;
}

RenderAPI::Texture2D * Device::CreateScaledTexture2D(RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, char* fileBuffer, unsigned int fileLengh)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "usage=%d, width=%d, height=%d", usage, width, height);

	if (width == 0 || height == 0)
	{
		LOG_FUNCTION_FAILED_INVALID_CALL(m_pAPIInstance, "Width and Height cannot be 0.");
		return NULL;
	}

	bool managed = m_pAPIInstance->IsSupportManaged();
	bool dynamic = m_supportDynamicTexture;
	FormatTextureUsage(usage, dynamic, managed);

	unsigned int d3dUsage = dynamic ? D3DUSAGE_DYNAMIC : 0;
	D3DPOOL d3dPool = managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;

	D3DXIMAGE_INFO info;
	info.Width = width;
	info.Height = height;
	info.Depth = 1;
	info.MipLevels = 1;

	IDirect3DTexture9* pTexture = NULL;
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(m_pDevice,
		fileBuffer, fileLengh,
		width, height, 1,
		d3dUsage, D3DFMT_FROM_FILE, d3dPool,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0x00000000,		//no color key
		&info,			//Pointer to a D3DXIMAGE_INFO structure 
		NULL,			//Pointer to a PALETTEENTRY structure
		&pTexture);

	if (hr == S_OK)
	{
		::Texture2D* texture = NULL;

		//Textures created with D3DPOOL_DEFAULT are not lockable.
		//Textures created in video memory are lockable when created with USAGE_DYNAMIC.
		//D3DLOCK_DISCARD, is only valid when the resource is created with USAGE_DYNAMIC.
		if (dynamic || managed)
		{
			texture = new ::Texture2D(pTexture, GetRenderAPIFormat(info.Format), usage, managed, info.Width, info.Height, false, *m_pAPIInstance);
		}
		else
		{
			texture = new ::NoLockableTexture2D(pTexture, GetRenderAPIFormat(info.Format), usage, managed, info.Width, info.Height, false, *m_pAPIInstance);
		}
		return texture;
	}
	else
	{
		return NULL;
	}
}



RenderAPI::TextureCube * Device::CreateTextureCube(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int edgeLength, unsigned int layer, bool autoGenMipmaps)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "usage=%d, format=%d, edge length=%d, layer=%d, auto generate mipmaps=%d",
		usage, format, edgeLength, layer, autoGenMipmaps);

	if (edgeLength == 0)
	{
		LOG_FUNCTION_FAILED_INVALID_CALL(m_pAPIInstance, "Edge Length cannot be 0.");
		return NULL;
	}

	if (layer == 1)
	{
		autoGenMipmaps = false;
	}

	bool managed = m_pAPIInstance->IsSupportManaged();
	bool dynamic = m_supportDynamicTexture;
	FormatTextureUsage(usage, dynamic, managed);

	IDirect3DCubeTexture9* pTexture = NULL;
	unsigned int d3dUsage = autoGenMipmaps ? D3DUSAGE_AUTOGENMIPMAP : 0;
	if (dynamic) d3dUsage |= D3DUSAGE_DYNAMIC;
	D3DPOOL d3dPool = managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT;
	HRESULT hr = m_pDevice->CreateCubeTexture(edgeLength, layer, d3dUsage, s_TextureFormats[format], d3dPool, &pTexture, NULL);

	if (hr != S_OK)
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateCubeTexture Failed.", hr);
		return NULL;
	}

	::TextureCube* texture = new ::TextureCube(pTexture, format, usage, managed, edgeLength, autoGenMipmaps, *m_pAPIInstance);
	return texture;
}

RenderAPI::FXEffect * Device::CreateFXEffectFromFile(const char * effectFilePath, const char * includeDir)
{
	LOG_FUNCTION_CALL(*m_pAPIInstance);

	ID3DXEffect* pEffect = NULL;
	AutoR<ID3DXBuffer> pErrorBuffer;
	EffectInclude includeCallback(includeDir);
	DWORD flags = D3DXSHADER_USE_LEGACY_D3DX9_31_DLL; //要想支持ps 1_x 需要使用这个。


	HRESULT hr = D3DXCreateEffectFromFileA(m_pDevice, effectFilePath, 0, &includeCallback, flags, 0, &pEffect, &pErrorBuffer);
	if (SUCCEEDED(hr))
	{
		//CacheNames();
		//pEffect->SetStateManager(pDeviceD3D->GetStateManager());
		//读fx文件
		//LogInfo(ENGINE_INIT, OutPut_File, "LoadFxFromDisk:读fxo文件:%s", fxofilename.c_str());
		pEffect->SetStateManager(m_pAPIInstance->pContext->GetStateManager());
		return new FXEffect(pEffect, m_pAPIInstance->GetRenderStatistic());
	}
	else
	{
		if (pErrorBuffer.IsNotNullPtr())
		{
			std::string errorStr = (char*)pErrorBuffer->GetBufferPointer();
			LOG_FUNCTION_FAILED_DETAIL(m_pAPIInstance, "D3DXCreateEffectFromFileA Failed.", errorStr.c_str(), hr);
		}
		return NULL;
	}

}

RenderAPI::RenderTarget* Device::CreateRenderTarget(RenderAPI::TextureFormat format, unsigned int width, unsigned int height)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "format=%d, width=%d, height=%d", format, width, height);

	D3DFORMAT rtFormat = s_TextureFormats[format];
	IDirect3DTexture9* pTexture = NULL;
	HRESULT hr = m_pDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, rtFormat, D3DPOOL_DEFAULT, &pTexture, NULL);

	if (hr != S_OK)
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateTexture Failed.", hr);
		return NULL;
	}

	return new RenderTarget(pTexture, format, width, height, *m_pAPIInstance);
}

RenderAPI::DepthStencil * Device::CreateDepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
{
	return CreateDepthStencilImplement(format, width, height);
}

RenderAPI::OcclusionQuery* Device::CreateOcclusionQuery()
{
	LOG_FUNCTION_CALL(*m_pAPIInstance);

	if (m_pAPIInstance->IsSupportOcclusionQuery())
	{
		IDirect3DQuery9* pOcclusionQuery = NULL;
		if (S_OK == m_pDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &pOcclusionQuery))
		{
			return new OcclusionQuery(pOcclusionQuery);
		}
	}
	return NULL;
}

RenderAPI::TextBox * Device::CreateTextBox(int screen_x, int screen_y, int width, int height)
{
	LOG_FUNCTION_PARAM(*m_pAPIInstance, "screen_x=%d, screen_y=%d, width=%d, height=%d", screen_x, screen_y, width, height);

	ID3DXFont* pFont = NULL;

	HRESULT hr = D3DXCreateFontA(m_pDevice, -12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "System", &pFont);

	if (hr != S_OK)
	{
		return NULL;
	}
	else
	{
		TextBox* tB = new TextBox(pFont, screen_x, screen_y, width, height);
		return tB;
	}


}

unsigned int Device::AddReference()
{
	return ++m_refCount;
}

::DepthStencil* Device::CreateDepthStencilImplement(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
{
	IDirect3DSurface9* pDSSurface = NULL;
	HRESULT hr = m_pDevice->CreateDepthStencilSurface(width, height, s_DSFormats[format], D3DMULTISAMPLE_NONE, 0, TRUE, &pDSSurface, NULL);
	if (hr != S_OK)
	{
		LOG_FUNCTION_FAILED_ERRCODE(m_pAPIInstance, "CreateDepthStencilSurface Failed.", hr);
		return NULL;
	}

	return new DepthStencil(pDSSurface, format, width, height);
}

void Device::Release()
{
	LOG_FUNCTION_CALL(*m_pAPIInstance);

	if (0 == --m_refCount)
	{
		delete this;
	}
}





