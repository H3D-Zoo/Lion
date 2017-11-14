#include "SwapChain.h"

SwapChain::SwapChain(::DepthStencil* dsSurface, const RenderAPI::SwapChainDesc & swapChainDesc)
	: m_pRenderTarget(NULL)
	, m_pDepthStencil(dsSurface)
{
	InitRenderTarget(swapChainDesc.backbufferFormat, swapChainDesc.backbufferWidth, swapChainDesc.backbufferHeight);
}

SwapChain::~SwapChain()
{
	m_pDepthStencil->Release();
	m_pRenderTarget->Release();

	m_pDepthStencil = NULL;
	m_pRenderTarget = NULL;
}

RenderAPI::RenderTarget* SwapChain::GetRenderTarget()
{
	m_pRenderTarget->AddReference();
	return m_pRenderTarget;
}

RenderAPI::DepthStencil* SwapChain::GetDepthStencil()
{
	m_pDepthStencil->AddReference();
	return m_pDepthStencil;
}

unsigned int SwapChain::GetWidth() const
{
	return m_pRenderTarget->GetWidth();
}

unsigned int SwapChain::GetHeight() const
{
	return m_pRenderTarget->GetHeight();
}

bool SwapChain::OnResize(unsigned int width, unsigned int height)
{
	if (width > 0 && height > 0)
	{
		m_pRenderTarget->Resize(width, height);
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

unsigned int SwapChain::AddReference()
{
	return ++m_refCount;
}

void SwapChain::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}


void SwapChain::InitRenderTarget(RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height)
{
	m_pRenderTarget = new ::RenderTarget(format, width, height);
}
