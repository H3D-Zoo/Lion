#include "SwapChain.h"

SwapChain::SwapChain(const RenderAPI::SwapChainDesc & swapChainDesc)
	: m_renderTarget(swapChainDesc.backbufferFormat, swapChainDesc.backbufferWidth, swapChainDesc.backbufferHeight, false)
	, m_depthStencil(swapChainDesc.zbufferFormat, swapChainDesc.backbufferWidth, swapChainDesc.backbufferHeight, false)
{

}

RenderAPI::RenderTarget* SwapChain::GetRenderTarget()
{
	m_renderTarget.AddRef();
	return &m_renderTarget;
}

RenderAPI::DepthStencil * SwapChain::GetDepthStencil()
{
	m_depthStencil.AddRef();
	return &m_depthStencil;
}

unsigned int SwapChain::GetWidth() const
{
	return m_renderTarget.GetWidth();
}

unsigned int SwapChain::GetHeight() const
{
	return m_renderTarget.GetHeight();
}

bool SwapChain::OnResize(unsigned int width, unsigned int height)
{
	if (width > 0 && height > 0)
	{
		m_renderTarget.Resize(width, height);
		return true;
	}
	else
	{
		return false;
	}
}

RenderAPI::DeviceState SwapChain::Present()
{
	return RenderAPI::DEVICE_OK;
}

void SwapChain::Release()
{
	if (m_refCount.OnlyOne())
	{
		delete this;
	}
	else
	{
		--m_refCount;
	}
}

void SwapChain::AddRef()
{
	++m_refCount;
}
