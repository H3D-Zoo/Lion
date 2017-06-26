#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RenderAPIImpl.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "DX9Include.h"


class SwapChain : public RenderAPI::SwapChain
{
public:
	SwapChain(IDirect3DSwapChain9* swapChain, const RenderAPI::SwapChainDesc & swapChainDesc);

	SwapChain::SwapChain(IDirect3DSwapChain9* swapChain, ::DepthStencil* dsSurface, const RenderAPI::SwapChainDesc & swapChainDesc);

	~SwapChain();

	virtual RenderAPI::RenderTarget* GetRenderTarget();

	virtual RenderAPI::DepthStencil* GetDepthStencil();

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool OnResize(unsigned int width, unsigned int height);

	virtual void Present();

	virtual void Release();

	void AddRef();

private:
	void InitRenderTarget(IDirect3DSwapChain9* swapChain, RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height);

	RefCount m_refCount;
	bool m_isFullscreen;
	::RenderTarget* m_pRenderTarget;
	::DepthStencil* m_pDepthStencil;
	IDirect3DSwapChain9* m_pSwapChain;
};