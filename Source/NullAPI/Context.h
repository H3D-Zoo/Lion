#pragma once

#include "../../RenderAPI/RenderAPI.h"

class Context :public RenderAPI::Context
{
public:
	virtual void ClearRenderTarget(RenderAPI::RenderTarget* rt, unsigned int color);

	virtual void ClearDepthStencil(RenderAPI::DepthStencil* ds, float z, unsigned int stencil);

	virtual void SetViewport(const RenderAPI::Viewport&);

	virtual void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget);

	virtual void SetDepthStencil(RenderAPI::DepthStencil* depthStencil);

	virtual void SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount);

	virtual void SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset);

	virtual void SetTexture(unsigned int slot, RenderAPI::Texture2D* texture);

	virtual void SetBlendState(const RenderAPI::BlendState& state);

	virtual void SetAlphaTestingState(const RenderAPI::AlphaTestingState& state);

	virtual void SetDepthTestingState(const RenderAPI::DepthTestingState& state);

	virtual void SetStencilTestingState(const RenderAPI::StencilTestingState& state);

	virtual void SetDepthWriting(bool enable);

	virtual void SetTextureBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	virtual void SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler);

	virtual void SetScissorState(const RenderAPI::ScissorState& state);

	virtual void SetFillMode(RenderAPI::FillMode mode);

	virtual void SetCullMode(RenderAPI::CullMode mode);

	virtual void SetDepthBias(float bias);

	virtual void SetTextureFactor(unsigned int factor);

	virtual bool BeginScene();

	virtual void EndScene();

	virtual void Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount);

	virtual RenderAPI::DeviceState CheckDeviceLost();

	virtual bool ResetDevice();

	virtual void Release();
};