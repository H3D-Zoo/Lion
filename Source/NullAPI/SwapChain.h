#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RenderTarget.h"
#include "DepthStencil.h"



class SwapChain : public RenderAPI::SwapChain
{
public:
	SwapChain::SwapChain(::DepthStencil* dsSurface, const RenderAPI::SwapChainDesc & swapChainDesc);

	~SwapChain();

	virtual RenderAPI::RenderTarget* GetRenderTarget();

	virtual RenderAPI::DepthStencil* GetDepthStencil();

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool OnResize(unsigned int width, unsigned int height);

	virtual RenderAPI::DeviceState Present();

	virtual unsigned int AddReference();

	virtual void Release();

private:
	void InitRenderTarget(RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height);

	RefCount m_refCount;
	bool m_isFullscreen;
	::RenderTarget* m_pRenderTarget;
	::DepthStencil* m_pDepthStencil;
};