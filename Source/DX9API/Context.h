#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include <map>
#include "StateManager.h"
#include "DX9Include.h"

class APIGlobal;

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

private:
	IDirect3DDevice9* m_pDevice;
	std::vector<IDirect3DSurface9*> m_pCurrentRTs;
	IDirect3DSurface9* m_pCurrentDS;
	IDirect3DSurface9* m_pDefaultRT;
	IDirect3DSurface9* m_pDefaultDS;

};

class Context :public RenderAPI::Context
{
public:
	Context(APIGlobal* pAPIContext, IDirect3DDevice9* device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS);

	~Context();

	virtual RenderAPI::DeviceCaps GetDeviceCaps();

	virtual unsigned int GetAvailableTextureMemory();

	virtual void ClearRenderTarget(unsigned int color);

	virtual void ClearDepthBuffer(float z);

	virtual void ClearStencilBuffer(unsigned int stencil);

	virtual void SetViewport(const RenderAPI::Viewport&);

	virtual void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget);

	virtual void SetDepthStencil(RenderAPI::DepthStencil* depthStencil);

	virtual void SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount);

	virtual void SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset);

	virtual void SetTexture(unsigned int slot, RenderAPI::Texture2D* texture);

	virtual void SetBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetBlendState() const;

	virtual void SetAlphaTestingState(const RenderAPI::AlphaTestingState& state);

	virtual RenderAPI::AlphaTestingState GetAlphaTestingState() const ;

	virtual void SetDepthTestingState(const RenderAPI::DepthTestingState& state);

	virtual RenderAPI::DepthTestingState GetDepthTestingState() const;

	virtual void SetStencilTestingState(const RenderAPI::StencilTestingState& state);

	virtual RenderAPI::StencilTestingState GetStencilTestingState() const;

	virtual void SetDepthWriting(bool enable);

	virtual void SetTextureBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	virtual void SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler);

	virtual void SetScissorState(const RenderAPI::ScissorState& state);

	virtual RenderAPI::ScissorState GetScissorState() const;

	virtual void SetFillMode(RenderAPI::FillMode mode);

	virtual void SetCullMode(RenderAPI::CullMode mode);

	virtual void SetDepthBias(float bias);

	virtual void SetTextureFactor(unsigned int factor);

	virtual bool BeginScene();

	virtual void EndScene();

	virtual void Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount);

	virtual RenderAPI::DeviceState Context::Present();

	virtual RenderAPI::DeviceState CheckDeviceLost();

	virtual RenderAPI::DeviceState ResetDevice(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual void Release();

	ID3DXEffectStateManager* GetStateManager();

private:
	void SetVertexElements(int index, const RenderAPI::VertexElement* s, int count);
	void RebuildDecalration();

	APIGlobal* m_pAPI;
	FXStateManager m_renderStateManager;
	IDirect3DDevice9* m_pDevice;
	IDirect3DDevice9Ex* m_pDeviceEx;
	BackBufferManager m_backBufferManager;
	unsigned int m_vertexCount;
	unsigned int m_indexBufferOffset;

	struct VertexDecl
	{
		int Count;
		std::vector<RenderAPI::VertexElement> Elements;
		bool Set(const RenderAPI::VertexElement* s, int count);
		bool Clear();
	};

	bool m_vertexDeclChanged;
	unsigned int m_vertexDeclCacheCount;
	std::vector<VertexDecl> m_vertexDeclCache;
	std::vector<D3DVERTEXELEMENT9> m_d3dDeclaration;
	IDirect3DVertexDeclaration9* m_pVertexDeclaration;
	typedef std::map<std::vector<D3DVERTEXELEMENT9>, IDirect3DVertexDeclaration9*> VertexDecalrationPool;
	VertexDecalrationPool m_vertexDeclarationPool;

	mutable RenderAPI::ScissorState m_scissorState;
};