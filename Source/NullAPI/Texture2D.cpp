#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"

Texture2D::Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height)
	: m_texFormat(format)
	, m_usage(usage)
	, m_texWidth(width)
	, m_texHeight(height)
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

RenderAPI::MappedResource Texture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	return RenderAPI::MappedResource();
}

void Texture2D::UnlockRect(unsigned int layer)
{

}

void Texture2D::GenerateMipmaps()
{
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
