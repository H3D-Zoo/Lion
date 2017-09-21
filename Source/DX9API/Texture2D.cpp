#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"

Texture2D::Texture2D(APIInstance* pAPIInstance, IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage,
	unsigned int width, unsigned int height,
	bool autoGenMipmaps, bool recreateWhenDeviceLost, bool isRenderTexture)
	: m_pAPIInstance(pAPIInstance)
	, m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_recreateWhenDeviceLost(recreateWhenDeviceLost)
	, m_isRenderTexture(isRenderTexture)
	, m_pTexture(texture)
	, m_pTempTextureForUpdate(NULL)
	, m_texWidth(width)
	, m_texHeight(height)
{
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

IDirect3DTexture9 ** Texture2D::TextureForUpdate(unsigned int index)
{
	if (m_pTempTextureForUpdate.size() <= index)
		m_pTempTextureForUpdate.resize(index + 1);
	return &(m_pTempTextureForUpdate[index]);
}

RenderAPI::MappedResource Texture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;
	if (m_isRenderTexture)
	{
		m_pAPIInstance->LogError("Texture2D::Lock", " Render Texture cannot be locked because.");
		ret.Success = false;
	}
	else if (m_usage == RenderAPI::RESUSAGE_Immuable)
	{
		m_pAPIInstance->LogError("Texture2D::Lock", "Immuable Texture cannot be locked.");
		ret.Success = false;
	}
	else if (m_usage == RenderAPI::RESUSAGE_Dynamic || m_usage == RenderAPI::RESUSAGE_DynamicRW)
	{
		D3DLOCKED_RECT lockedRect;
		HRESULT hr = m_pTexture->LockRect(layer, &lockedRect, NULL, GetLockOption(lockOption, m_usage));
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
	}
	else
	{
		ret.Success = false;
		IDirect3DDevice9* pDevice = NULL;
		HRESULT hr = m_pTexture->GetDevice(&pDevice);
		if (hr == S_OK)
		{
			IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(layer);
			if (pTextureForUpdate != NULL)
			{
				pTextureForUpdate->Release();
			}

			hr = pDevice->CreateTexture(m_texWidth, m_texHeight, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &pTextureForUpdate, NULL);
			if (hr == S_OK)
			{
				D3DLOCKED_RECT lockedRect;
				if (S_OK == pTextureForUpdate->LockRect(layer, &lockedRect, NULL, GetLockOption(lockOption, m_usage)))
				{
					ret.Success = true;
					ret.DataPtr = lockedRect.pBits;
					ret.LinePitch = lockedRect.Pitch;
				}
				else
				{
					m_pAPIInstance->LogError("Texture2D::Lock", "helper texture lock failed.", hr);
					ret.Success = false;
				}
			}
			else
			{
				m_pAPIInstance->LogError("Texture2D::Lock", "helper texture creation failed.", hr);
				ret.Success = false;
			}
		}
	}
	return ret;
}

void Texture2D::UnlockRect(unsigned int layer)
{
	if (m_usage == RenderAPI::RESUSAGE_Immuable)
	{
	}
	else if (m_usage == RenderAPI::RESUSAGE_Dynamic || m_usage == RenderAPI::RESUSAGE_DynamicRW)
	{
		m_pTexture->UnlockRect(layer);
	}
	else
	{
		IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(layer);

		if (pTextureForUpdate != NULL)
		{
			HRESULT hr = pTextureForUpdate->UnlockRect(layer);
			if (S_OK == hr)
			{
				IDirect3DDevice9* pDevice = NULL;
				hr = m_pTexture->GetDevice(&pDevice);
				if (hr == S_OK)
				{
					IDirect3DSurface9 *pSurfaceSrc, *pSurfaceDst;
					pTextureForUpdate->GetSurfaceLevel(0, &pSurfaceSrc);
					m_pTexture->GetSurfaceLevel(0, &pSurfaceDst);
					hr = pDevice->UpdateSurface(pSurfaceSrc, NULL, pSurfaceDst, NULL);
					if (hr != S_OK)
					{
						m_pAPIInstance->LogError("Texture2D::UnlockRect", "helper texture update failed.", hr);
					}
					pSurfaceSrc->Release();
					pSurfaceDst->Release();
				}

			}

			pTextureForUpdate->Release();
			pTextureForUpdate = NULL;
		}
	}
}

void Texture2D::GenerateMipmaps()
{
	m_pTexture->GenerateMipSubLevels();
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
	return m_recreateWhenDeviceLost;
}

bool Texture2D::AutoGenMipmaps() const
{
	return m_autoGenMipmaps;
}

bool Texture2D::IsCubemap() const
{
	return false;
}

bool Texture2D::IsRenderTexture() const
{
	return m_isRenderTexture;
}

bool Texture2D::SaveToFile(const char * fileName, RenderAPI::ImageFormat format)
{
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
