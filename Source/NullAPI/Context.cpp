#include "Context.h"
#include "APIInstance.h"
#include "Device.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "AutoPtr.hpp"
#include "Texture2D.h"
#include "TextureCube.h"

Context::Context(RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS, RenderStatistic& renderStatic)
	: m_backBufferManager(defRT, defDS, renderStatic)
	, m_nNXCacheFVF(0)
	, m_renderStatistic(renderStatic)
{
	m_backBufferManager.SetMaxTextureStage(16);
}

const RenderAPI::DeviceCaps& Context::GetDeviceCaps() const
{
	return m_deviceCaps;
}

unsigned int Context::GetAvailableTextureMemory() const
{
	const unsigned int size = 1024 * 1024;
	return size;
}

void Context::ClearRenderTarget(unsigned int color)
{

}

void Context::ClearDepthBuffer(float z)
{

}

void Context::ClearStencilBuffer(unsigned int stencil)
{

}

void Context::SetViewport(const RenderAPI::Viewport& vp)
{
	m_currentViewport = vp;
}

RenderAPI::Viewport Context::GetViewport()
{
	return m_currentViewport;
}

void Context::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget)
{
	m_backBufferManager.SetRenderTarget(index, renderTarget);
}

void Context::SetDepthStencil(RenderAPI::DepthStencil* depthStencil)
{
	m_backBufferManager.SetDepthStencil(depthStencil);
}

void Context::SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount)
{
	m_vertexBufferCount = 0;
	for (unsigned int i = 0; i < bufferCount; i++)
	{
		if (buffers[i].BufferPtr != NULL)
		{
			//统计信息
			//m_renderStatistic.OnSetSourceStream(pVertexBuffer);
		}
	}
}

void Context::SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset)
{
	if (buffer != NULL)
	{
		//m_renderStatistic.OnSetIndexBuffer(pIndexBuffer);
	}
}

void Context::SetVertexDeclaration(RenderAPI::VertexDeclaration * decl)
{
	m_renderStatistic.OnSetVertexDeclaration();
}

void Context::SetTexture(unsigned int slot, RenderAPI::Texture* texture)
{
	m_renderStatistic.OnSetTexture(texture);
}

void Context::SetBlendState(const RenderAPI::BlendState& state)
{
	m_currentBlendState = state;
}

void Context::SetAlphaSeparateBlendState(const RenderAPI::BlendState& state)
{
	m_currentAlphaSeparateBlendState = state;
}

RenderAPI::BlendState Context::GetBlendState() const
{
	return m_currentBlendState;
}

RenderAPI::BlendState Context::GetAlphaSeparateBlendState() const
{
	return m_currentAlphaSeparateBlendState;
}

void Context::SetAlphaTestingState(const RenderAPI::AlphaTestingState& state)
{
	m_currentAlphaTestingState = state;
}

RenderAPI::AlphaTestingState Context::GetAlphaTestingState() const
{
	return m_currentAlphaTestingState;
}

void Context::SetDepthTestingState(const RenderAPI::DepthTestingState& state)
{
	m_currentDepthTestingState = state;
}

RenderAPI::DepthTestingState Context::GetDepthTestingState() const
{
	return m_currentDepthTestingState;
}
void Context::SetClipPlaneState(bool enable)
{
	m_currentClipPlaneState = enable;
}
bool Context::GetClipPlaneState() const
{
	return m_currentClipPlaneState;
}
void Context::SetStencilTestingState(const RenderAPI::StencilTestingState& state)
{
	m_currentStencilTestingState = state;
}

RenderAPI::StencilTestingState Context::GetStencilTestingState() const
{
	return m_currentStencilTestingState;
}

void Context::SetDepthWriting(bool enable)
{

}

void Context::SetTextureColorBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state)
{

}

void Context::SetTextureAlphaBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state)
{

}

void Context::SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler)
{

}

void Context::SetScissorState(const RenderAPI::ScissorState& state)
{
	m_currentScissorState = state;
}

RenderAPI::ScissorState Context::GetScissorState() const
{
	return m_currentScissorState;
}

void Context::SetColorWriteMask(bool r, bool g, bool b, bool a)
{

}

void Context::SetFillMode(RenderAPI::FillMode mode)
{
	m_currentFillMode = mode;
}

RenderAPI::FillMode Context::GetFillMode() const
{
	return m_currentFillMode;
}

void Context::SetCullMode(RenderAPI::CullMode mode)
{
	m_currentCullMode = mode;
}

RenderAPI::CullMode Context::GetCullMode() const
{
	return m_currentCullMode;
}

void Context::SetDepthBias(float bias)
{

}

void Context::SetSlopScaleDepthBias(float bias)
{

}

void Context::SetTextureFactor(unsigned int factor)
{

}

bool Context::BeginScene()
{
	return true;
}

void Context::EndScene()
{
}

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startVertex, unsigned int primitiveCount)
{
	m_renderStatistic.OnDraw(primitive, primitiveCount);
}

void Context::DrawWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int primitiveCount, const void* pVertexData, unsigned int vertexStride)
{
	m_renderStatistic.OnDrawUp(primitive, primitiveCount);
}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount)
{
	m_renderStatistic.OnDrawIndexed(primitive, primitiveCount);
}

void Context::DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned int* pIndexData, const void* pVertexData, unsigned int vertexStride)
{
	m_renderStatistic.OnDrawIndexedUp(primitive, primitiveCount);
}

void Context::DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned short* pIndexData, const void* pVertexData, unsigned int vertexStride)
{
	m_renderStatistic.OnDrawIndexedUp(primitive, primitiveCount);
}

bool Context::UpdateTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst)
{
	if (src == NULL || dst == NULL ||
		dst->IsRenderTexture() ||
		src->GetWidth() != dst->GetWidth() ||
		src->GetHeight() != dst->GetHeight() ||
		src->GetFormat() != dst->GetFormat())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Context::StretchTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst, RenderAPI::SamplerFilter filter)
{
	return true;
}

RenderAPI::DeviceState Context::Present()
{
	return RenderAPI::DEVICE_OK;
}

RenderAPI::DeviceState Context::GetState()
{
	return RenderAPI::DEVICE_OK;
}

RenderAPI::DeviceState Context::ResetDevice(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync)
{
	return RenderAPI::DEVICE_OK;
}

void Context::EvictManagedResources()
{

}

const RenderAPI::RenderStatisticsData& Context::GetRenderStatisticsData() const
{
	return m_renderStatistic.GetData();
}

void Context::ClearRenderStatisticsData()
{
	m_renderStatistic.Reset();
}

unsigned int Context::AddReference()
{
	return ++m_refCount;
}

void Context::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void* Context::GetImplementPtr()
{
	return NULL;
}

BackBufferManager::BackBufferManager(RenderAPI::RenderTarget * defRT, RenderAPI::DepthStencil * defDS, RenderStatistic& renderStatistic)
	: m_pCurrentRTs(1)
	, m_pDefaultRT(NULL)
	, m_pDefaultDS(NULL)
	, m_renderStatistic(renderStatistic)
{
	m_pCurrentRTs[0] = defRT;
	m_pCurrentDS = defDS;

	m_pDefaultRT = m_pCurrentRTs[0];
	m_pDefaultDS = m_pCurrentDS;
}

void BackBufferManager::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget * rt)
{
	//当index为0时，rt为空的含义被视为使用主窗体的默认backbuffer
	//因为manager有对rt做Cache的需求，我们用defaultRT来代替NULL作为参数使用
	//遇到此情况会把NULL替换为defaultRT
	RenderAPI::RenderTarget* renderTarget = rt == NULL ? m_pDefaultRT : rt;
	

	if (index >= m_pCurrentRTs.size())
	{
		EnlargeCurrentRTVector(index + 1);
	}

	m_pCurrentRTs[index] = renderTarget;
}


void BackBufferManager::EnlargeCurrentRTVector(unsigned int count)
{
	size_t oldSize = m_pCurrentRTs.size();
	size_t newSize = count;
	for (; oldSize < newSize; oldSize++)
	{
		m_pCurrentRTs.push_back(NULL);
	}
}

void BackBufferManager::SetDepthStencil(RenderAPI::DepthStencil * ds)
{
	m_pCurrentDS = ds == NULL ? m_pDefaultDS : ds;
}

bool BackBufferManager::IsDefaultRT()
{
	return m_pDefaultRT == m_pCurrentRTs[0];
}

bool BackBufferManager::IsDefaultDS()
{
	return m_pDefaultDS == m_pCurrentDS;
}

void BackBufferManager::ResetDefaultDepthStencil(RenderAPI::DepthStencil * defDS)
{
	m_pCurrentDS = defDS;
	m_pDefaultDS = m_pCurrentDS;
}

void BackBufferManager::ResetDefaultRenderTarget(RenderAPI::RenderTarget * defRT)
{
	m_pCurrentRTs[0] = defRT;
	m_pDefaultRT = m_pCurrentRTs[0];
}

void BackBufferManager::SetMaxTextureStage(unsigned int maxStage)
{
	m_maxTextureStage = maxStage;
}

bool operator != (const RenderAPI::VertexElement& left, const RenderAPI::VertexElement& right)
{
	return  (left.SemanticName != right.SemanticName ||
		left.SemanticIndex != right.SemanticIndex ||
		left.AlignOffset != right.AlignOffset ||
		left.Format != right.Format);
}