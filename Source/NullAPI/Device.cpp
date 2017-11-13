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
#include "OcclusionQuery.h"
#include "TextBox.h"

Device::Device(APIInstance& api, const RenderAPI::SwapChainDesc & desc, bool isFullscreen, bool useVerticalSync)
	: m_apiInstance(api)
	, m_pDefaultSwapChain(NULL)
{
	m_apiInstance.AddRef();

	::DepthStencil* pDepthStencil = new ::DepthStencil(desc.zbufferFormat, desc.backbufferWidth, desc.backbufferHeight);
	m_pDefaultSwapChain = new ::SwapChain(pDepthStencil, desc);
}

Device::~Device()
{
	m_pDefaultSwapChain->Release();
	m_pDefaultSwapChain = NULL;

	m_apiInstance.Release();
}

RenderAPI::SwapChain * Device::GetDefaultSwapChain()
{
	m_pDefaultSwapChain->AddReference();
	return m_pDefaultSwapChain;
}

RenderAPI::SwapChain * Device::CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc & swapChainDesc)
{
	::DepthStencil* ds = new ::DepthStencil(
		swapChainDesc.zbufferFormat,
		swapChainDesc.backbufferWidth,
		swapChainDesc.backbufferHeight);

	return new ::SwapChain(ds, swapChainDesc);
}

RenderAPI::VertexBuffer* Device::CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize)
{
	if (vertexCount == 0 || vertexSize == 0)
	{
		m_apiInstance.LogError("CreateVertexBuffer", "Vertex Count and Vertex Size cannot be 0.");
		return NULL;
	}
	else
	{
		return new VertexBuffer(usage, vertexCount, vertexSize, m_apiInstance);
	}

}

RenderAPI::IndexBuffer* Device::CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount)
{
	if (indexCount == 0)
	{
		m_apiInstance.LogError("CreateIndexBuffer", "Index Count cannot be 0.");
		return NULL;
	}
	else
	{
		return new IndexBuffer(usage, format, indexCount);
	}
}

int  formatLength[] =
{
	sizeof(float) ,
	sizeof(float) * 2 ,
	sizeof(float) * 3 ,
	sizeof(float) * 4 ,
	sizeof(unsigned char) * 4 ,
	sizeof(unsigned char) * 4 ,
	sizeof(short) * 2 ,
	sizeof(short) * 4 ,
	sizeof(unsigned char) * 4 ,
	sizeof(short) * 2 ,
	sizeof(short) * 4 ,
	sizeof(unsigned short) * 2 ,
	sizeof(unsigned short) * 4 ,
};

RenderAPI::VertexDeclaration* Device::CreateVertexDeclaration(const RenderAPI::VertexElement * elements, unsigned int elementCount)
{
	if (elementCount == 0 || elements == NULL)
	{
		m_apiInstance.LogError("CreateVertexDeclaration", "Element Count cannot be 0.");
		return NULL;
	}
	else
	{
		//构建 Vertex Element 数组
		std::vector<int> alignOffsets;
		std::vector<RenderAPI::VertexElement> elementList(elements, elements + elementCount);

		for (unsigned int i = 0; i < elementCount; i++)
		{
			RenderAPI::VertexElement& ve = elementList[i];
			while (alignOffsets.size() <= ve.StreamIndex)
			{
				alignOffsets.push_back(0);
			}
			int& offset = alignOffsets[ve.StreamIndex];
			ve.AlignOffset = ve.AlignOffset == 0xFFFFFFFF ? offset : ve.AlignOffset;
			offset += formatLength[ve.Format];
		}

		return new VertexDeclaration(elementList);
	}
}

RenderAPI::Texture2D * Device::CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps)
{
	if (width == 0 || height == 0)
	{
		m_apiInstance.LogError("CreateTexture", "Width and Height cannot be 0.");
		return NULL;
	}
	else
	{
		if (layer == 1)
		{
			autoGenMipmaps = false;
		}

		return new ::Texture2D(format, usage, width, height, layer, autoGenMipmaps);
	}
}

RenderAPI::Texture2D * Device::CreateScaledTexture2D(RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, char* fileBuffer, unsigned int fileLengh)
{
	return CreateTexture2D(usage, RenderAPI::TEX_Unknown, width, height, 1, false);
}

RenderAPI::TextureCube * Device::CreateTextureCube(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int edgeLength, unsigned int layer, bool autoGenMipmaps)
{
	if (edgeLength == 0)
	{
		m_apiInstance.LogError("CreateTextureCube", "edgeLength cannot be 0.");
		return NULL;
	}
	else
	{
		if (layer == 1)
		{
			autoGenMipmaps = false;
		}
		return new ::TextureCube(format, usage, edgeLength, autoGenMipmaps);
	}
}

RenderAPI::FXEffect * Device::CreateFXEffectFromFile(const char * effectFilePath, const char * includeDir)
{
	return new FXEffect(m_apiInstance.renderStatistic);
}

RenderAPI::RenderTarget* Device::CreateRenderTarget(RenderAPI::TextureFormat format, unsigned int width, unsigned int height)
{
	return new RenderTarget(format, width, height);
}

RenderAPI::DepthStencil * Device::CreateDepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
{
	return new ::DepthStencil(format, width, height);
}

RenderAPI::OcclusionQuery* Device::CreateOcclusionQuery()
{
	return new OcclusionQuery();
}

RenderAPI::TextBox * Device::CreateTextBox(int screen_x, int screen_y, int width, int height)
{
	return new TextBox(screen_x, screen_y, width, height);
}

unsigned int Device::AddReference()
{
	return ++m_refCount;
}

void Device::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}
