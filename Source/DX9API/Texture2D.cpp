#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"

Texture2D::Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height)
	: m_texFormat(format)
	, m_usage(usage)
	, m_pTexture(texture)
	, m_texWidth(width)
	, m_texHeight(height)
{

}

Texture2D::Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, void* initialData)
	: m_texFormat(format)
	, m_usage(usage)
	, m_pTexture(texture)
	, m_texWidth(width)
	, m_texHeight(height)
{

}

Texture2D::~Texture2D()
{
	m_pTexture->Release();
	m_pTexture = NULL;
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

void Texture2D::Release()
{
	delete this;
}

void Texture2D::Resize(unsigned int width, unsigned int height)
{
	m_texWidth = width;
	m_texHeight = height;
}

IDirect3DTexture9 * Texture2D::GetD3DTexture() { return m_pTexture; }

RenderAPI::MappedResource Texture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;
	if (m_usage == RenderAPI::RESUSAGE_Immuable)
		return ret;


	D3DLOCKED_RECT lockedRect;
	if (S_OK == m_pTexture->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]))
	{
		ret.Success = true;
		ret.DataPtr = lockedRect.pBits;
		ret.LinePitch = lockedRect.Pitch;
	}
	return ret;
}

void Texture2D::UnlockRect(unsigned int layer)
{
	m_pTexture->UnlockRect(layer);
}