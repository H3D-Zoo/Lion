#include "RenderTarget.h"

RenderTarget::RenderTarget(RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_rtTexture(NULL)
{
	RenderAPI::TextureFormat texFormat = (format == RenderAPI::RT_ARGB8) ? RenderAPI::TEX_ARGB : RenderAPI::TEX_XRGB;
	m_rtTexture = new Texture2D(texFormat, RenderAPI::RESUSAGE_StaticWO, width, height, 1, false);
}

RenderTarget::RenderTarget(RenderAPI::TextureFormat format, unsigned int width, unsigned int height)
	: m_format(RenderAPI::RT_RenderTexture)
	, m_width(width)
	, m_height(height)
	, m_rtTexture(NULL)
{
	m_rtTexture = new Texture2D(format, RenderAPI::RESUSAGE_StaticWO, width, height, 1, false);
}

RenderTarget::~RenderTarget()
{
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

}

void RenderTarget::Reset(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat)
{
	Resize(width, height);
	m_format = rtFormat;
}