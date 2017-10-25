#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"

Texture2D::Texture2D(APIInstance* pAPIInstance, IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged,
	unsigned int width, unsigned int height, bool autoGenMipmaps)
	: m_pAPIInstance(pAPIInstance)
	, m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_isManaged(isManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_pTexture(texture)
	, m_texWidth(width)
	, m_texHeight(height)
	, ClearStamp(0)
{
	m_pAPIInstance->AddRef();
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
	m_surfaces.clear();
	m_pTexture->Release();
	m_pAPIInstance->Release();
	m_pTexture = NULL;
	m_pAPIInstance = NULL;

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

void Texture2D::AddRef()
{
	++m_refCount;
}

IDirect3DTexture9 * Texture2D::GetD3DTexture()
{
	return m_pTexture;
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
	D3DLOCKED_RECT lockedRect;
	HRESULT hr = m_pTexture->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]);
	if (S_OK == hr)
	{
		ret.Success = true;
		ret.DataPtr = lockedRect.pBits;
		ret.LinePitch = lockedRect.Pitch;
	}
	else
	{
		m_pAPIInstance->LogError("Texture2D::Lock", " Lock failed.", hr);
		ret.Success = false;
	}

	return ret;
}

void Texture2D::UnlockRect(unsigned int layer)
{
	m_pTexture->UnlockRect(layer);
}

void Texture2D::SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter)
{
	m_pTexture->SetAutoGenFilterType(s_d3dSamplerFilter[filter]);
}

void Texture2D::GenerateMipmaps()
{
	if (!m_autoGenMipmaps)
	{
		m_pTexture->GenerateMipSubLevels();
	}
}

RenderAPI::TextureSurface* Texture2D::GetSurface(unsigned int index)
{
	if (m_surfaces.size() <= index)
	{
		m_surfaces.resize(index + 1);
	}

	if (m_surfaces[index] == NULL)
	{
		return m_surfaces[index];
	}
	else
	{
		IDirect3DSurface9* pSurface = NULL;
		if (S_OK == m_pTexture->GetSurfaceLevel(index, &pSurface))
		{
			m_surfaces[index] = new TextureSurface(this, pSurface);
		}
	}

	if (m_surfaces[index])
	{
		m_surfaces[index]->AddRef();
	}
	return m_surfaces[index];
}

unsigned int Texture2D::GetLayerCount() const
{
	return m_pTexture->GetLevelCount();
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
	if (format == RenderAPI::IMG_ERR)
		return false;

	return S_OK == D3DXSaveTextureToFileA(fileName, s_d3dxFileFormat[format], NULL, NULL);
}

TextureSurface::TextureSurface(Texture2D* pTexture, IDirect3DSurface9 * pSurface)
	: m_pParentTexture(pTexture)
	, m_pSurface(pSurface)
	, m_hDC(NULL)
{

}

TextureSurface::~TextureSurface()
{
	ReleaseDC();
	m_pSurface->Release();
}

void* TextureSurface::GetDC()
{
	if (m_hDC == NULL)
	{
		m_pSurface->GetDC(&m_hDC);
	}
	return m_hDC;
}

void TextureSurface::ReleaseDC()
{
	if (m_hDC != NULL)
	{
		m_pSurface->ReleaseDC(m_hDC);
		m_hDC = NULL;
	}
}

bool TextureSurface::SaveToFile(const char * fileName, RenderAPI::ImageFormat format)
{
	if (format == RenderAPI::IMG_ERR)
		return false;

	return S_OK == D3DXSaveSurfaceToFile(fileName, s_d3dxFileFormat[format], m_pSurface, NULL, NULL);
}

void TextureSurface::Release()
{
	m_pParentTexture->Release();
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void TextureSurface::AddRef()
{
	m_pParentTexture->AddRef();
	++m_refCount;
}

IDirect3DSurface9 * TextureSurface::GetD3DTextureSurfacePtr()
{
	return m_pSurface;
}