#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "DX9Include.h"


class BackBufferManager
{
public:
	BackBufferManager(IDirect3DDevice9* device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS);

	~BackBufferManager();

	void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* rt);

	void SetDepthStencil(RenderAPI::DepthStencil* rt);

private:
	IDirect3DDevice9* m_pDevice;
	std::vector<IDirect3DSurface9*> m_pCurrentRTs;
	IDirect3DSurface9* m_pCurrentDS;
};

class Context :public RenderAPI::Context
{
public:
	Context(IDirect3DDevice9* device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS);

	~Context();

	virtual void ClearRenderTarget(RenderAPI::RenderTarget* rt, unsigned int color);

	virtual void ClearDepthStencil(RenderAPI::DepthStencil* ds, float z, unsigned int stencil);

	virtual void SetViewport(const RenderAPI::Viewport&);

	virtual void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget);

	virtual void SetRenderTarget(RenderAPI::DepthStencil* depthStencil);

	virtual void SetVertexBuffers(unsigned int startSlot, RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount);

	virtual void SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset);

	virtual void SetEffect(RenderAPI::FXEffect*);

	virtual void SetTextures(unsigned int startSlot, RenderAPI::Texture2D** textures, unsigned int resCount);

	virtual void SetBlendState(const RenderAPI::BlendState& state);

	virtual void SetAlphaTestingState(const RenderAPI::AlphaTestingState& state);

	virtual void SetDepthTestingState(const RenderAPI::DepthTestingState& state);

	virtual void SetStencilTestingState(const RenderAPI::StencilTestingState& state);

	virtual void SetDepthWriting(bool enable);

	virtual void SetTextureBlendingState(unsigned int startSlot, const RenderAPI::TextureBlendingState** samplers, unsigned int count);

	virtual void SetTextureSampler(unsigned int startSlot, const RenderAPI::TextureSampler** samplers, unsigned int count);

	virtual void SetScissorState(const RenderAPI::ScissorState& state);

	virtual void SetFillMode(RenderAPI::FillMode mode);

	virtual void SetCullMode(RenderAPI::CullMode mode);

	virtual void SetDepthBias(float bias);

	virtual void Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount);

	virtual void GenerateMipmaps(RenderAPI::Texture2D* texture);

	virtual void CheckDeviceLost();

	virtual void ResetDevice();

	virtual void Release();

private:
	IDirect3DDevice9* m_pDevice;
	BackBufferManager m_backBufferManager;
	unsigned int m_vertexBufferCount;
};