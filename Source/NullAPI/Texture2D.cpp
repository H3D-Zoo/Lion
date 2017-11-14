#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Context.h"

Texture2D::Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps)
	: m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_isManaged(usage == RenderAPI::RESUSAGE_DynamicManaged || usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_StaticWOManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_texWidth(width)
	, m_texHeight(height)
	, m_layerCount(layer)
	, ClearStamp(0)
{
	m_buffer.resize(LinePitch() * m_texHeight);
}

Texture2D::~Texture2D()
{
	std::vector<TextureSurface*>::iterator itCur = m_surfaces.begin();
	std::vector<TextureSurface*>::iterator itEnd = m_surfaces.end();
	for (; itCur != itEnd; ++itCur)
	{
		TextureSurface* pSurface = *itCur;
		if (pSurface != NULL)
		{
			pSurface->Release();
		}
	}
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

unsigned int Texture2D::GetLength() const
{
	return GetTextureLength(m_texWidth, m_texHeight, m_texFormat, m_autoGenMipmaps);
}

void Texture2D::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void Texture2D::Resize(unsigned int width, unsigned int height)
{
	m_texWidth = width;
	m_texHeight = height;
}

unsigned int Texture2D::AddReference()
{
	return ++m_refCount;
}

unsigned int Texture2D::LinePitch() const
{
	switch (m_texFormat)
	{
	default:
	case RenderAPI::TEX_ARGB: 
	case RenderAPI::TEX_XRGB:
	case RenderAPI::TEX_D24S8:
	case RenderAPI::TEX_D24X8:
		return 4 * m_texWidth;
	case RenderAPI::TEX_DXT1:
		return 8 * m_texWidth;
	case RenderAPI::TEX_DXT3:
	case RenderAPI::TEX_DXT5:
		return 16 * m_texWidth;
	case RenderAPI::TEX_D32:
		return 4 * m_texWidth;
	case RenderAPI::TEX_D16:
		return 2 * m_texWidth;
	case RenderAPI::TEX_R32F:
		return 4 * m_texWidth;
	case RenderAPI::TEX_RG32F:
		return 8 * m_texWidth;
	case RenderAPI::TEX_ARGB32F:
		return 16 * m_texWidth;
	};
}

RenderAPI::MappedResource Texture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	//D3DLOCK_DISCARD, is only valid when the resource is created with USAGE_DYNAMIC.
	if (lockOption == RenderAPI::LOCK_NoOverWrite ||
		(!m_isDynamic && lockOption == RenderAPI::LOCK_Discard))
	{
		lockOption = RenderAPI::LOCK_Normal;
	}

	RenderAPI::MappedResource ret;

	ret.Success = true;
	ret.DataPtr = &(m_buffer[0]);
	ret.LinePitch = LinePitch();

	return ret;
}

void Texture2D::UnlockRect(unsigned int layer)
{

}

void Texture2D::SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter)
{

}

void Texture2D::GenerateMipmaps()
{

}

RenderAPI::TextureSurface* Texture2D::GetSurface(unsigned int index)
{
	if (m_surfaces.size() <= index)
	{
		size_t oldSize = m_surfaces.size();
		size_t newSize = index + 1;
		m_surfaces.resize(newSize);
		for (size_t i = oldSize; i < newSize; i++)
		{
			m_surfaces[i] = NULL;
		}
	}

	if (m_surfaces[index] == NULL)
	{
		m_surfaces[index] = new TextureSurface(this);
	}

	if (m_surfaces[index])
	{
		m_surfaces[index]->AddReference();
	}
	return m_surfaces[index];
}

unsigned int Texture2D::GetLayerCount() const
{
	return m_layerCount;
}

bool Texture2D::NeedRecreateWhenDeviceLost() const
{
	return !m_isManaged;
}

bool Texture2D::AutoGenMipmaps() const
{
	return m_autoGenMipmaps;
}

bool Texture2D::SaveToFile(const char * fileName, RenderAPI::ImageFormat format)
{
	return false;
}

TextureSurface::TextureSurface(Texture2D* pTexture)
	: m_pParentTexture(pTexture)
{

}

void* TextureSurface::GetDC()
{
	return NULL;
}

void TextureSurface::ReleaseDC()
{

}

bool TextureSurface::SaveToFile(const char * fileName, RenderAPI::ImageFormat format)
{
	return false;
}

void TextureSurface::Release()
{
	if (m_pParentTexture != NULL)
	{
		m_pParentTexture->Release();
	}

	if (0 == --m_refCount)
	{
		delete this;
	}
}

unsigned int TextureSurface::AddReference()
{
	if (m_pParentTexture != NULL)
	{
		m_pParentTexture->AddReference();
	}
	return ++m_refCount;
}