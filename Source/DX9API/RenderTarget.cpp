#include "RenderTarget.h"

namespace
{
	RenderAPI::TextureFormat s_rtTexFormats[] =
	{
		RenderAPI::TEX_XRGB,
		RenderAPI::TEX_ARGB,
	};
}

RenderTarget::RenderTarget(IDirect3DSurface9* rtSurface, RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_rtSurface(rtSurface)
	, m_rtTexture(NULL)
{

}

RenderTarget::RenderTarget(IDirect3DTexture9* rtTexture, RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_rtSurface(NULL)
	, m_rtTexture(NULL)
{
	rtTexture->GetSurfaceLevel(0, &m_rtSurface);
	m_rtTexture = new Texture2D(rtTexture, s_rtTexFormats[format], RenderAPI::RESUSAGE_Default, width, height, true);
}

RenderTarget::~RenderTarget()
{
	m_rtSurface->Release();
	m_rtSurface = NULL;

	if (m_rtTexture != NULL)
	{
		m_rtTexture->Release();
		m_rtTexture = NULL;
	}
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

bool RenderTarget::IsTexture2D() const
{
	return false;
}

RenderAPI::Texture2D* RenderTarget::GetTexturePtr()
{
	return m_rtTexture;
}

void RenderTarget::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void RenderTarget::AddRef()
{
	++m_refCount;
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

IDirect3DSurface9 * RenderTarget::GetD3DSurface() const { return m_rtSurface; }
