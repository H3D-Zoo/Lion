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

void Context::SetViewport(const RenderAPI::Viewport&)
{

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

void Context::SetTexture(unsigned int slot, RenderAPI::Texture2D* texture)
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

void Context::SetTextureBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state)
{

}

void Context::SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler)
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

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount)
{

}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount)
{

}

RenderAPI::DeviceState Context::Present()
{
	return RenderAPI::DEVICE_OK;
}

RenderAPI::DeviceState Context::CheckDeviceLost()
{
	return RenderAPI::DEVICE_OK;
}

RenderAPI::DeviceState Context::ResetDevice()
{
	return RenderAPI::DEVICE_OK;
}

void Context::Release()
{
	delete this;
}