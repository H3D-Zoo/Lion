#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"

namespace
{
	RenderAPI::ZBufferFormat s_ZFormats[4] =
	{
		RenderAPI::ZBUFFER_D24S8,
		RenderAPI::ZBUFFER_D24X8,
		RenderAPI::ZBUFFER_D16
	};
}

Texture2D::Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, bool rt, bool ds)
	: m_texFormat(format)
	, m_usage(usage)
	, m_texWidth(width)
	, m_texHeight(height)
	, m_isRenderTarget(rt)
	, m_isDepthStencil(ds)
	, m_renderTarget(NULL)
	, m_depthStencil(NULL)
{
	if (rt)
	{
		RenderAPI::BackBufferFormat fmt;
		if (RenderAPI::TEX_XRGB)
			fmt = RenderAPI::BACKBUFFER_XRGB8;
		else if (RenderAPI::TEX_ARGB)
			fmt = RenderAPI::BACKBUFFER_ARGB8;

		m_renderTarget = new RenderTarget(fmt, width, height);
	}

	if (ds)
	{
		m_depthStencil = new DepthStencil(s_ZFormats[format - RenderAPI::TEX_D24S8], width, height);
	}
}

Texture2D::Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, void* initialData, bool rt, bool ds)
	: m_texFormat(format)
	, m_usage(usage)
	, m_texWidth(width)
	, m_texHeight(height)
	, m_isRenderTarget(rt)
	, m_isDepthStencil(ds)
	, m_renderTarget(NULL)
	, m_depthStencil(NULL)
{

}

RenderAPI::TextureFormat Texture2D::GetFormat() const
{
	return m_texFormat;
}

unsigned int Texture2D::GetWidth() const
{
	return m_texWidth;
}

unsigned int Texture2D::GetHeight() const
{
	return m_texHeight;
}

bool Texture2D::IsRenderTarget() const
{
	return m_isRenderTarget;
}

RenderAPI::RenderTarget * Texture2D::GetRenderTarget() const
{
	return m_renderTarget;
}

bool Texture2D::IsDepthStencil() const
{
	return m_isDepthStencil;
}

RenderAPI::DepthStencil * Texture2D::GetDepthStencil() const
{
	return m_depthStencil;
}

void Texture2D::Release()
{
	delete this;
}

void Texture2D::Resize(unsigned int width, unsigned int height)
{
	m_texWidth = width;
	m_texHeight = height;
}
