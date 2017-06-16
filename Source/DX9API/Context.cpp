#include "Context.h"
#include "RenderTarget.h"
#include "DepthStencil.h"

Context::Context(IDirect3DDevice9 * device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS)
	: m_pDevice(device)
	, m_backBufferManager(device, defRT, defDS)
{
}

Context::~Context()
{
	m_pDevice->Release();
	m_pDevice = NULL;
}

void Context::ClearRenderTarget(RenderAPI::RenderTarget* rt, unsigned int color)
{
	m_backBufferManager.SetRenderTarget(0, rt);
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 0, 0);
}

void Context::ClearDepthStencil(RenderAPI::DepthStencil* ds, float z, unsigned int stencil)
{
	m_backBufferManager.SetDepthStencil(ds);
	m_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, z, stencil);
}

void Context::SetViewport(const RenderAPI::Viewport&)
{

}

void Context::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget)
{

}

void Context::SetRenderTarget(RenderAPI::DepthStencil* depthStencil)
{

}

void Context::SetVertexBuffers(unsigned int startSlot, RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount)
{

}

void Context::SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset)
{

}

void Context::SetEffect(RenderAPI::FXEffect*)
{

}

void Context::SetTextures(unsigned int startSlot, RenderAPI::Texture2D** textures, unsigned int resCount)
{

}

void Context::SetBlendState(const RenderAPI::BlendState& state)
{

}

void Context::SetAlphaTestingState(const RenderAPI::AlphaTestingState& state)
{

}

void Context::SetDepthTestingState(const RenderAPI::DepthTestingState& state)
{

}

void Context::SetStencilTestingState(const RenderAPI::StencilTestingState& state)
{

}

void Context::SetDepthWriting(bool enable)
{

}

void Context::SetTextureBlendingState(unsigned int startSlot, const RenderAPI::TextureBlendingState** samplers, unsigned int count)
{

}

void Context::SetTextureSampler(unsigned int startSlot, const RenderAPI::TextureSampler** samplers, unsigned int count)
{

}

void Context::SetScissorState(const RenderAPI::ScissorState& state)
{

}

void Context::SetFillMode(RenderAPI::FillMode mode)
{

}

void Context::SetCullMode(RenderAPI::CullMode mode)
{

}

void Context::SetDepthBias(float bias)
{

}

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int baseVertex, unsigned int primitiveCount)
{

}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertex, unsigned int primitiveCount)
{

}

void Context::GenerateMipmaps(RenderAPI::Texture2D* texture)
{

}

void Context::CheckDeviceLost()
{

}

void Context::ResetDevice()
{

}

void Context::Release()
{
	delete this;
}

BackBufferManager::BackBufferManager(IDirect3DDevice9 * device, RenderAPI::RenderTarget * defRT, RenderAPI::DepthStencil * defDS)
	: m_pDevice(device)
	, m_pCurrentRTs(1)
{
	m_pDevice->AddRef();
	m_pCurrentRTs[0] = ((::RenderTarget*)defRT)->GetD3DSurface();
	m_pCurrentDS = ((::DepthStencil*)defDS)->GetD3DSurface();
}

BackBufferManager::~BackBufferManager()
{
	m_pDevice->Release();
}

void BackBufferManager::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget * rt)
{
	IDirect3DSurface9* rtSurface = ((::RenderTarget*)rt)->GetD3DSurface();

	if (index >= m_pCurrentRTs.size())
	{
		m_pCurrentRTs.resize(index + 1);
		m_pDevice->SetRenderTarget(index, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
	else if (rtSurface != m_pCurrentRTs[index])
	{
		m_pDevice->SetRenderTarget(0, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
}

void BackBufferManager::SetDepthStencil(RenderAPI::DepthStencil * rt)
{
}
