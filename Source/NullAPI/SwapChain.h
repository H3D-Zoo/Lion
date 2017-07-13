#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"
#include "RenderTarget.h"
#include "DepthStencil.h"


class SwapChain : public RenderAPI::SwapChain
{
public:
	SwapChain(const RenderAPI::SwapChainDesc & swapChainDesc);

	virtual RenderAPI::RenderTarget* GetRenderTarget();

	virtual RenderAPI::DepthStencil* GetDepthStencil();

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool OnResize(unsigned int width, unsigned int height);

	virtual RenderAPI::DeviceState Present();

	virtual void Release();

	void AddRef();

private:
	RefCount m_refCount;
	::RenderTarget m_renderTarget;
	::DepthStencil m_depthStencil;
};