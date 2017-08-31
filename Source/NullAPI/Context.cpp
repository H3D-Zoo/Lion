#include "Context.h"

RenderAPI::DeviceCaps Context::GetDeviceCaps()
{
	return RenderAPI::DeviceCaps();
}

unsigned int Context::GetAvailableTextureMemory()
{
	return 0;
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
	m_viewport = vp;
}

RenderAPI::Viewport Context::GetViewport()
{
	return m_viewport;
}

void Context::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget)
{

}

void Context::SetDepthStencil(RenderAPI::DepthStencil* depthStencil)
{

}

void Context::SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount)
{

}

void Context::SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset)
{

}

void Context::SetVertexDeclaration(RenderAPI::VertexDeclaration * decl) 
{

}

void Context::SetTexture(unsigned int slot, RenderAPI::Texture2D* texture)
{

}

void Context::SetBlendState(const RenderAPI::BlendState& state)
{
	m_blendState = state;
}

RenderAPI::BlendState Context::GetBlendState() const
{
	return m_blendState;
}

void Context::SetAlphaSeparateBlendState(const RenderAPI::BlendState & state)
{
	m_alphaSepareteBlendState = state;
}

RenderAPI::BlendState Context::GetAlphaSeparateBlendState() const
{
	return m_alphaSepareteBlendState;
}

void Context::SetAlphaTestingState(const RenderAPI::AlphaTestingState& state)
{
	m_alphaTestState = state;
}

RenderAPI::AlphaTestingState Context::GetAlphaTestingState() const
{
	return m_alphaTestState;
}

void Context::SetDepthTestingState(const RenderAPI::DepthTestingState& state)
{
	m_depthTestState = state;
}

RenderAPI::DepthTestingState Context::GetDepthTestingState() const
{
	return m_depthTestState;
}

void Context::SetStencilTestingState(const RenderAPI::StencilTestingState& state)
{
	m_stencilTestState = state;
}

RenderAPI::StencilTestingState Context::GetStencilTestingState() const
{
	return m_stencilTestState;
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
	m_scissorState = state;
}

RenderAPI::ScissorState Context::GetScissorState() const
{
	return m_scissorState;
}

void Context::SetColorWriteMask(bool r, bool g, bool b, bool a)
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

}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount)
{

}

bool Context::UpdateTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst)
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

void Context::Release()
{
	delete this;
}