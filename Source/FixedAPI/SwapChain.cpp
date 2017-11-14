#include "SwapChain.h"
#include "EnumMapping.h"

SwapChain::SwapChain(IDirect3DSwapChain9* swapChain, ::DepthStencil* dsSurface, const RenderAPI::SwapChainDesc & swapChainDesc, IInternalLogger& logger)
	: m_pRenderTarget(NULL)
	, m_pDepthStencil(dsSurface)
	, m_pSwapChain(swapChain)
{
	IDirect3DSurface9* pBackBuffer = NULL;
	m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pRenderTarget = new ::RenderTarget(pBackBuffer, 
		swapChainDesc.backbufferFormat, 
		swapChainDesc.backbufferWidth, 
		swapChainDesc.backbufferHeight, 
		logger);
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
	// possible return values should be D3D_OK or D3DERR_DEVICEREMOVED
	// if D3DERR_DEVICEREMOVED is returned, we should recreate D3DDevice.
	// but it seems be occured in device9ex situation.
	HRESULT hr = m_pSwapChain->Present(NULL, NULL, NULL, NULL, NULL);
	return DeviceStateMapping(hr);
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

void SwapChain::ReleaseSurfaceWhenLost()
{
	m_pRenderTarget->ReleaseWhenDeviceLost();
	m_pDepthStencil->ReleaseWhenDeviceLost();
}

void SwapChain::ResetBackBuffers(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, RenderAPI::DepthStencilFormat dsFormat, IDirect3DSurface9* pDSSurafce)
{
	IDirect3DSurface9* pBackBuffer = NULL;
	m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	m_pRenderTarget->Reset(width, height, rtFormat, pBackBuffer);
	m_pDepthStencil->Reset(width, height, dsFormat, pDSSurafce);
}