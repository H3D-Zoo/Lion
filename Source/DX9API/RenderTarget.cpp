#include "RenderTarget.h"

namespace
{
	RenderAPI::TextureFormat s_rtTexFormats[] =
	{
		RenderAPI::TEX_XRGB,
		RenderAPI::TEX_ARGB,
	};
}

RenderTarget::RenderTarget(IDirect3DSurface9* rtSurface, RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_rtSurface(rtSurface)
	, m_rtTexture(NULL)
{

}

RenderTarget::RenderTarget(IDirect3DTexture9* rtTexture, RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_rtSurface(NULL)
	, m_rtTexture(NULL)
{
	rtTexture->GetSurfaceLevel(0, &m_rtSurface);
	m_rtTexture = new Texture2D(rtTexture, s_rtTexFormats[format], RenderAPI::RESUSAGE_Default, width, height);
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

RenderAPI::BackBufferFormat RenderTarget::GetFormat() const
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
	if (m_refCount.OnlyOne())
	{
		delete this;
	}
	else
	{
		--m_refCount;
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

IDirect3DSurface9 * RenderTarget::GetD3DSurface() const { return m_rtSurface; }
