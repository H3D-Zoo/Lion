#include <stdlib.h>
#include "Device.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "FXEffect.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "OcclusionQuery.h"

Device::Device(const RenderAPI::SwapChainDesc & desc, bool isFullscreen, bool useVerticalSync)
	: m_defaultSwapChain(desc)
{

}

RenderAPI::SwapChain * Device::GetDefaultSwapChain()
{
	m_defaultSwapChain.AddRef();
	return &m_defaultSwapChain;
}

RenderAPI::SwapChain * Device::CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc & swapChainDesc)
{
	return new ::SwapChain(swapChainDesc);
}

RenderAPI::VertexBuffer* Device::CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::VertexElement* elements, unsigned int elementCount, void * initialData)
{
	if (vertexCount == 0 || vertexSize == 0 || elementCount == 0 || elements == NULL)
	{
		return NULL;
	}

	return new VertexBuffer(usage, vertexCount, vertexSize, elements, elementCount);
}

RenderAPI::IndexBuffer* Device::CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount, void * initialData)
{
	if (indexCount == 0)
	{
		return NULL;
	}

	return new IndexBuffer(usage, format, indexCount);
}

RenderAPI::Texture2D * Device::CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, void* initialData, int dataLinePitch, int dataHeight)
{
	if (width == 0 || height == 0)
	{
		return NULL;
	}

	return new Texture2D(format, usage, width, height);
}

RenderAPI::FXEffect * Device::CreateFXEffectFromFile(const char * effectFilePath)
{
	return new FXEffect();
}

RenderAPI::RenderTarget * Device::CreateRenderTarget(RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
{
	return new RenderTarget(format, width, height, true);
}

RenderAPI::DepthStencil * Device::CreateDepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height)
{
	return new DepthStencil(format, width, height, true);
}

RenderAPI::OcclusionQuery* Device::CreateOcclusionQuery()
{
	return new OcclusionQuery();
}

void* Device::GetImplementPtr()
{
	return NULL;
}

void Device::Release()
{
	delete this;
}