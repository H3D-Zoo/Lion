#include "RenderTarget.h"
#include "Context.h"

RenderTarget::RenderTarget(IDirect3DSurface9* rtSurface, RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height, RenderAPI::Logger& logger)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_rtSurface(rtSurface)
	, m_rtTexture(NULL)
{
	RenderAPI::TextureFormat texFormat = (format == RenderAPI::RT_ARGB8) ? RenderAPI::TEX_ARGB : RenderAPI::TEX_XRGB;
	m_rtTexture = new RenderSurface2D(rtSurface, texFormat, width, height, logger);
}

RenderTarget::RenderTarget(IDirect3DTexture9* rtTexture, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, RenderAPI::Logger& logger)
	: m_format(RenderAPI::RT_RenderTexture)
	, m_width(width)
	, m_height(height)
	, m_rtSurface(NULL)
	, m_rtTexture(NULL)
{
	rtTexture->GetSurfaceLevel(0, &m_rtSurface);
	m_rtTexture = new RenderTexture2D(rtTexture, format, RenderAPI::RESUSAGE_StaticWO, width, height, logger);
}

RenderTarget::~RenderTarget()
{
	m_rtSurface->Release();
	m_rtSurface = NULL;

	m_rtTexture->Release();
	m_rtTexture = NULL;
}

RenderAPI::RenderTargetFormat RenderTarget::GetFormat() const
{
	return m_format;
}

unsigned int RenderTarget::GetWidth() const
{
	return m_width;
}

unsigned int RenderTarget::GetHeight() const
{
	return m_height;
}

RenderAPI::Texture2D* RenderTarget::GetTexturePtr()
{
	if (m_rtTexture != NULL)
	{
		m_rtTexture->AddReference();
	}
	return m_rtTexture;
}

void RenderTarget::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

unsigned int RenderTarget::AddReference()
{
	return ++m_refCount;
}

void RenderTarget::Resize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
}

void RenderTarget::ReleaseWhenDeviceLost()
{
	m_rtSurface->Release();
	m_rtSurface = NULL;
}

void RenderTarget::Reset(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, IDirect3DSurface9 * pSurface)
{
	Resize(width, height);
	m_format = rtFormat;
	m_rtSurface = pSurface;
}

IDirect3DSurface9 * RenderTarget::GetD3DSurface() const 
{
	return m_rtSurface;
}
