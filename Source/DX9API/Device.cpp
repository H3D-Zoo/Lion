#include <stdlib.h>
#include "Device.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "FXEffect.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

Device::Device(IDirect3DDevice9* device, const RenderAPI::SwapChainDesc & desc, bool isFullscreen, bool useVerticalSync)
	: m_defaultSwapChain(device, desc, isFullscreen)
	, m_defaultDepthStencil(desc.zbufferFormat, desc.backbufferWidth, desc.backbufferHeight)
	, m_pDevice(device)
{

}

Device::~Device()
{
	m_pDevice->Release();
	m_pDevice = NULL;
}

RenderAPI::SwapChain * Device::GetDefaultSwapChain() const
{
	m_defaultSwapChain.AddRef();
	return &m_defaultSwapChain;
}

RenderAPI::DepthStencil * Device::GetDefaultDepthStencil() const
{
	m_defaultDepthStencil.AddRef();
	return &m_defaultDepthStencil;
}

RenderAPI::SwapChain * Device::CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc & swapChainDesc)
{
	return new ::SwapChain(m_pDevice,  swapChainDesc, false);
}

RenderAPI::VertexBuffer* Device::CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::Semantic * semantics, unsigned int semanticCount, void * initialData)
{
	if (vertexCount == 0 || vertexSize == 0 || semanticCount == 0 || semantics == NULL)
	{
		return NULL;
	}

	if (initialData == NULL)
	{
		return new VertexBuffer(usage, vertexCount, vertexSize, semantics, semanticCount);
	}
	else
	{
		return new VertexBuffer(usage, vertexCount, vertexSize, semantics, semanticCount, initialData);
	}
}

RenderAPI::IndexBuffer* Device::CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount, void * initialData)
{
	if (indexCount == 0)
	{
		return NULL;
	}

	if (initialData == NULL)
	{
		return new IndexBuffer(usage, format, indexCount);
	}
	else
	{
		return new IndexBuffer(usage, format, indexCount, initialData);
	}

}

RenderAPI::Texture2D * Device::CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, RenderAPI::TextureBinding binding, unsigned int width, unsigned int height, void * initialData)
{
	if (width == 0 || height == 0)
	{
		return NULL;
	}

	bool isRT = binding == RenderAPI::BINDING_RenderTraget;
	bool isDS = binding == RenderAPI::BINDING_DepthStencil;

	if (isRT)
	{
		if (format != RenderAPI::TEX_XRGB
			&& format != RenderAPI::TEX_ARGB)
		{
			return NULL;
		}
	}

	if (isDS)
	{
		if (format != RenderAPI::TEX_D24S8
			&& format != RenderAPI::TEX_D24X8
			&& format != RenderAPI::TEX_D32
			&& format != RenderAPI::TEX_D16)
		{
			return NULL;
		}
	}

	if (initialData == NULL)
	{
		return new Texture2D(format, usage, width, height, isRT, isDS);
	}
	else
	{
		return new Texture2D(format, usage, width, height, initialData, isRT, isDS);
	}
}

RenderAPI::FXEffect * Device::CreateFXEffectFromFile(const char * effectFilePath)
{
	return new FXEffect();
}

RenderAPI::RenderTarget * Device::CreateRenderTarget(RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
{
	return new RenderTarget(format, width, height);
}

RenderAPI::DepthStencil * Device::CreateDepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height)
{
	return new DepthStencil(format, width, height);
}

void Device::Release()
{
	delete this;
}