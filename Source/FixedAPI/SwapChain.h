#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "DX9Include.h"


class SwapChain : public RenderAPI::SwapChain
{
public:
	SwapChain::SwapChain(IDirect3DSwapChain9*, ::DepthStencil*, const RenderAPI::SwapChainDesc&, RenderAPI::Logger&);

	~SwapChain();

	virtual RenderAPI::RenderTarget* GetRenderTarget();

	virtual RenderAPI::DepthStencil* GetDepthStencil();

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool OnResize(unsigned int width, unsigned int height);

	virtual RenderAPI::DeviceState Present();

	virtual unsigned int AddReference();

	virtual void Release();

	void ReleaseSurfaceWhenLost();

	void ResetBackBuffers(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, RenderAPI::DepthStencilFormat dsFormat, IDirect3DSurface9* pDSSurafce);

private:
	RefCount m_refCount;
	bool m_isFullscreen;
	::RenderTarget* m_pRenderTarget;
	::DepthStencil* m_pDepthStencil;
	IDirect3DSwapChain9* m_pSwapChain;
};