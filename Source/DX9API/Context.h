#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include <map>
#include "StateManager.h"
#include "DX9Include.h"

class APIInstance;

class BackBufferManager
{
public:
	BackBufferManager(IDirect3DDevice9* device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS);

	~BackBufferManager();

	void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* rt);

	void SetDepthStencil(RenderAPI::DepthStencil* rt);

	bool IsDefaultRT();

	bool IsDefaultDS();

	void ResetDefaultDepthStencil(RenderAPI::DepthStencil* defRT);

	void ResetDefaultRenderTarget(RenderAPI::RenderTarget* defDS);

	void SetMaxTextureStage(unsigned int maxStage);

private:
	IDirect3DDevice9* m_pDevice;
	std::vector<IDirect3DSurface9*> m_pCurrentRTs;
	IDirect3DSurface9* m_pCurrentDS;
	IDirect3DSurface9* m_pDefaultRT;
	IDirect3DSurface9* m_pDefaultDS;
	unsigned int m_maxTextureStage;
};

class Context :public RenderAPI::Context, public RenderAPI::ContextLegacy
{
public:
	Context(APIInstance* pAPIContext, IDirect3DDevice9* device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS);

	~Context();

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

	virtual void SetVertexDeclaration(RenderAPI::VertexDeclaration* decl);

	virtual void SetTexture(unsigned int slot, RenderAPI::Texture* texture);

	virtual void SetBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetBlendState() const;

	virtual void SetAlphaSeparateBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetAlphaSeparateBlendState() const;

	virtual void SetAlphaTestingState(const RenderAPI::AlphaTestingState& state);

	virtual RenderAPI::AlphaTestingState GetAlphaTestingState() const ;

	virtual void SetDepthTestingState(const RenderAPI::DepthTestingState& state);

	virtual RenderAPI::DepthTestingState GetDepthTestingState() const;
	virtual void SetClipPlaneState(bool isEnable);

	virtual bool GetClipPlaneState() const;

	virtual void SetStencilTestingState(const RenderAPI::StencilTestingState& state);

	virtual RenderAPI::StencilTestingState GetStencilTestingState() const;

	virtual void SetDepthWriting(bool enable);

	virtual void SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler);

	virtual void SetScissorState(const RenderAPI::ScissorState& state);

	virtual RenderAPI::ScissorState GetScissorState() const;

	virtual void SetColorWriteMask(bool r, bool g, bool b, bool a);

	virtual void SetFillMode(RenderAPI::FillMode mode);

	virtual RenderAPI::FillMode GetFillMode() const;

	virtual void SetCullMode(RenderAPI::CullMode mode);

	virtual RenderAPI::CullMode GetCullMode() const;

	virtual void SetDepthBias(float bias);

	virtual void SetSlopScaleDepthBias(float bias);

	virtual void SetTextureFactor(unsigned int factor);

	virtual bool BeginScene();

	virtual void EndScene();

	virtual void Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int primitiveCount, const void* pVertexData, unsigned int vertexStride);

	virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned int* pIndexData, const void* pVertexData, unsigned int vertexStride);

	virtual void DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned short* pIndexData, const void* pVertexData, unsigned int vertexStride);

	virtual bool UpdateTexture(RenderAPI::Texture2D* src, RenderAPI::Texture2D* dst);

	virtual bool StretchTexture(RenderAPI::Texture2D* src, RenderAPI::Texture2D* dst, RenderAPI::SamplerFilter filter);

	virtual bool GetRenderTargetData(RenderAPI::RenderTarget* rt, RenderAPI::TextureSurface* surface);

	virtual bool GetDepthStencilData(RenderAPI::DepthStencil* ds, RenderAPI::TextureSurface* surface);

	virtual RenderAPI::DeviceState Context::Present();

	virtual RenderAPI::DeviceState GetState();

	virtual RenderAPI::DeviceState ResetDevice(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual void EvictManagedResources();

	virtual RenderAPI::ContextLegacy* GetContextLegacy();

	virtual void Release();

	// Context Legacy
	virtual void SetTextureColorBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	virtual void SetTextureAlphaBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	void DisableTexcoordTransform(unsigned int slot);

	virtual void ResetTexcoordIndex(unsigned int slot);

	virtual void DisableCustomShaderUsage();

	virtual void SetCustomFVF(unsigned int fvf);

	virtual void SetWorldMatrix(const float* matrix);

	virtual void SetViewMatrix(const float* matrix);

	virtual void SetProjectionMatrix(const float* matrix);

	virtual void SetTextureMatrix(unsigned int slot, const float* matrix);

	ID3DXEffectStateManager* GetStateManager();

private:
	APIInstance* m_pAPI;
	FXStateManager m_renderStateManager;
	IDirect3DDevice9* m_pDevice;
	IDirect3DDevice9Ex* m_pDeviceEx;
	BackBufferManager m_backBufferManager;
	unsigned int m_indexBufferOffset;
	unsigned int m_vertexBufferCount;
	mutable RenderAPI::ScissorState m_scissorState;
};