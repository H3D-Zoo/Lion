#include "DepthStencil.h"
#include <stdlib.h>
#include "Texture2D.h"

namespace
{
	RenderAPI::TextureFormat s_dsTexFormats[] =
	{
		RenderAPI::TEX_D24S8,
		RenderAPI::TEX_D24X8,
		RenderAPI::TEX_D16,
	};
}

DepthStencil::DepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height, bool isTexture)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_texture(NULL)
{
	if (isTexture)
	{
		m_texture = new ::Texture2D(s_dsTexFormats[format], RenderAPI::RESUSAGE_Default, width, height);
	}
}

DepthStencil::~DepthStencil()
{
	if (m_texture != NULL)
	{
		m_texture->Release();
		m_texture = NULL;
	}
}

RenderAPI::ZBufferFormat DepthStencil::GetFormat() const
{
	return m_format;
}

unsigned int DepthStencil::GetWidth() const
{
	return m_width;
}

unsigned int DepthStencil::GetHeight() const
{
	return m_height;
}

bool DepthStencil::IsTexture2D() const
{
	return m_texture != NULL;
}

RenderAPI::Texture2D * DepthStencil::GetTexturePtr()
{
	return m_texture;
}

void DepthStencil::Release()
{
	if (--m_refCount)
	{
		delete this;
	}
}

void DepthStencil::AddRef()
{
	++m_refCount;
}
