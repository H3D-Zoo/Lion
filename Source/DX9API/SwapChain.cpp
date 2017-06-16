#include "SwapChain.h"

SwapChain::SwapChain(IDirect3DDevice9* device, const RenderAPI::SwapChainDesc & swapChainDesc, bool fullscreen)
	: m_isFullscreen(fullscreen)
	, m_renderTarget(swapChainDesc.backbufferFormat, swapChainDesc.backbufferWidth, swapChainDesc.backbufferHeight)
	, m_pDevice(device)
{
	m_pDevice->AddRef();
}

SwapChain::~SwapChain()
{
	m_pDevice->Release();
}

RenderAPI::RenderTarget* SwapChain::GetRenderTarget() const
{
	m_renderTarget.AddRef();
	return &m_renderTarget;
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

void SwapChain::SetFullscreen(bool fullscreen)
{
	m_isFullscreen = fullscreen;
}

bool SwapChain::IsFullscreen() const
{
	return m_isFullscreen;
}

void SwapChain::Present()
{
	m_pDevice->Present(NULL, NULL, NULL, NULL);
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
