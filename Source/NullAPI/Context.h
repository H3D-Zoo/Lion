#pragma once

#include "../../RenderAPI/RenderAPI.h"

class Context :public RenderAPI::Context
{
public:
	virtual RenderAPI::DeviceCaps GetDeviceCaps();

	virtual unsigned int GetAvailableTextureMemory();

	virtual void ClearRenderTarget(unsigned int color);

	virtual void ClearDepthBuffer(float z);

	virtual void ClearStencilBuffer(unsigned int stencil);

	virtual void SetViewport(const RenderAPI::Viewport&);

	virtual RenderAPI::Viewport GetViewport();

	virtual void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget);

	virtual void SetDepthStencil(RenderAPI::DepthStencil* depthStencil);

	virtual void SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount);

	virtual void SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset);

	virtual void SetTexture(unsigned int slot, RenderAPI::Texture2D* texture);

	virtual void SetBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetBlendState() const;

	virtual void SetAlphaSeparateBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetAlphaSeparateBlendState() const;

	virtual void SetAlphaTestingState(const RenderAPI::AlphaTestingState& state);

	virtual RenderAPI::AlphaTestingState GetAlphaTestingState() const;

	virtual void SetDepthTestingState(const RenderAPI::DepthTestingState& state);

	virtual RenderAPI::DepthTestingState GetDepthTestingState() const;

	virtual void SetStencilTestingState(const RenderAPI::StencilTestingState& state);

	virtual RenderAPI::StencilTestingState GetStencilTestingState() const;

	virtual void SetDepthWriting(bool enable);

	virtual void SetTextureBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	virtual void SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler);

	virtual void SetScissorState(const RenderAPI::ScissorState& state);

	virtual RenderAPI::ScissorState GetScissorState() const;

	virtual void SetColorWriteMask(bool r, bool g, bool b, bool a);

	virtual void SetFillMode(RenderAPI::FillMode mode);

	virtual void SetCullMode(RenderAPI::CullMode mode);

	virtual void SetDepthBias(float bias);

	virtual void SetSlopScaleDepthBias(float bias);

	virtual void SetTextureFactor(unsigned int factor);

	virtual bool BeginScene();

	virtual void EndScene();

	virtual void Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount);

	virtual bool UpdateTexture(RenderAPI::Texture2D* src, RenderAPI::Texture2D* dst);

	virtual RenderAPI::DeviceState Present();

	virtual RenderAPI::DeviceState CheckDeviceLost();

	virtual RenderAPI::DeviceState ResetDevice(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual void Release();

private:
	RenderAPI::BlendState m_blendState;
	RenderAPI::BlendState m_alphaSepareteBlendState;
	RenderAPI::AlphaTestingState m_alphaTestState;
	RenderAPI::DepthTestingState m_depthTestState;
	RenderAPI::StencilTestingState m_stencilTestState;
	RenderAPI::ScissorState m_scissorState;
	RenderAPI::Viewport m_viewport;
};