#include "Context.h"

void Context::ClearRenderTarget(RenderAPI::RenderTarget* rt, unsigned int color)
{

}

void Context::ClearDepthStencil(RenderAPI::DepthStencil* ds, float z, unsigned int stencil)
{

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

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount)
{

}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount)
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