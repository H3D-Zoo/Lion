#include "RenderTarget.h"
#include <stdlib.h>
#include "Texture2D.h"

namespace
{
	RenderAPI::TextureFormat s_rtTexFormats[] =
	{
		RenderAPI::TEX_XRGB,
		RenderAPI::TEX_ARGB,
	};
}

RenderTarget::RenderTarget(RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height, bool isTexture)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_texture(NULL)
{
	if (isTexture)
	{
		m_texture = new ::Texture2D(s_rtTexFormats[format], RenderAPI::RESUSAGE_Default, width, height);
	}
}

RenderTarget::~RenderTarget()
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
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
	return m_texture != NULL;
}

RenderAPI::Texture2D * RenderTarget::GetTexturePtr()
{
	return m_texture;
}

void RenderTarget::Release()
{
	if (--m_refCount)
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
