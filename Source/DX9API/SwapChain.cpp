#include "SwapChain.h"
#include "EnumMapping.h"

SwapChain::SwapChain(IDirect3DSwapChain9* swapChain, ::DepthStencil* dsSurface, const RenderAPI::SwapChainDesc & swapChainDesc)
	: m_pRenderTarget(NULL)
	, m_pDepthStencil(dsSurface)
	, m_pSwapChain(swapChain)
{
	InitRenderTarget(swapChain, swapChainDesc.backbufferFormat, swapChainDesc.backbufferWidth, swapChainDesc.backbufferHeight);
}

SwapChain::~SwapChain()
{
	m_pDepthStencil->Release();
	m_pRenderTarget->Release();
	m_pSwapChain->Release();
	m_pDepthStencil = NULL;
	m_pRenderTarget = NULL;
	m_pSwapChain = NULL;
}

RenderAPI::RenderTarget* SwapChain::GetRenderTarget()
{
	m_pRenderTarget->AddRef();
	return m_pRenderTarget;
}

RenderAPI::DepthStencil* SwapChain::GetDepthStencil()
{
	m_pDepthStencil->AddRef();
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
	// possible return values should be D3D_OK or D3DERR_DEVICEREMOVED
	// if D3DERR_DEVICEREMOVED is returned, we should recreate D3DDevice.
	// but it seems be occured in device9ex situation.
	HRESULT hr = m_pSwapChain->Present(NULL, NULL, NULL, NULL, NULL);
	return DeviceStateMapping(hr);
}

void SwapChain::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void SwapChain::AddRef()
{
	++m_refCount;
}

void SwapChain::InitRenderTarget(IDirect3DSwapChain9 * swapChain, RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
{
	IDirect3DSurface9* pBackBuffer = NULL;
	m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pRenderTarget = new ::RenderTarget(pBackBuffer, format, width, height);
}
