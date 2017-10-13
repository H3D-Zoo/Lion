#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"

Texture2D::Texture2D(APIInstance* pAPIInstance, IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged,
	unsigned int width, unsigned int height,
	bool autoGenMipmaps, bool isRenderTexture)
	: m_pAPIInstance(pAPIInstance)
	, m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_isManaged(isManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_isRenderTexture(isRenderTexture)
	, m_pTexture(texture)
	, m_texWidth(width)
	, m_texHeight(height)
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
	ReleaseCopiedSystemTexture();
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
	ReleaseCopiedSystemTexture();
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void Texture2D::Resize(unsigned int width, unsigned int height)
{
	ReleaseCopiedSystemTexture();
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

IDirect3DTexture9* Texture2D::GetCopiedSystemTexture()
{
	IDirect3DDevice9* pDevice = NULL;
	m_pTexture->GetDevice(&pDevice);
	if (pDevice == NULL)
	{
		return NULL;
	}

	IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(0);
	if (pTextureForUpdate == NULL)
	{
		pDevice->CreateTexture(m_texWidth, m_texHeight, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &pTextureForUpdate, NULL);
	}

	if (pTextureForUpdate != NULL)
	{
		IDirect3DSurface9* pSrcSurface = NULL, *pDstSurface = NULL;
		m_pTexture->GetSurfaceLevel(0, &pSrcSurface);
		pTextureForUpdate->GetSurfaceLevel(0, &pDstSurface);

		if (pSrcSurface != NULL && pDstSurface != NULL)
		{
			pDevice->GetRenderTargetData(pSrcSurface, pDstSurface);
			pSrcSurface->Release();
			pDstSurface->Release();
		}
		else
		{
			if (pSrcSurface != NULL) pSrcSurface->Release();
			if (pDstSurface != NULL) pDstSurface->Release();
		}
	}
	return pTextureForUpdate;
}

void Texture2D::ReleaseCopiedSystemTexture()
{
	IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(0);
	if (pTextureForUpdate != NULL)
	{
		pTextureForUpdate->Release();
		pTextureForUpdate = NULL;
	}
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
		IDirect3DTexture9* pCopiedTexture = GetCopiedSystemTexture();
		if (pCopiedTexture != NULL)
		{
			D3DLOCKED_RECT lockedRect;
			HRESULT hr = pCopiedTexture->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]);
			if (hr == S_OK)
			{
				ret.DataPtr = lockedRect.pBits;
				ret.LinePitch = lockedRect.Pitch;
				ret.Success = false;
			}
			else
			{
				m_pAPIInstance->LogError("Texture2D::Lock", " Render Texture cannot be locked because.");
			}
		}
	}
	else if (m_isDynamic || m_isManaged)
	{
		//Textures created with D3DPOOL_DEFAULT are not lockable. Textures created in video memory are lockable when created with USAGE_DYNAMIC.
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
	}
	else
	{
		ret.Success = false;
		IDirect3DDevice9* pDevice = NULL;
		HRESULT hr = m_pTexture->GetDevice(&pDevice);
		if (hr == S_OK)
		{
			IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(layer);
			if (pTextureForUpdate == NULL)
			{
				hr = pDevice->CreateTexture(m_texWidth, m_texHeight, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &pTextureForUpdate, NULL);
			}
			if (hr == S_OK)
			{
				D3DLOCKED_RECT lockedRect;
				if (S_OK == pTextureForUpdate->LockRect(layer, &lockedRect, NULL, D3DLOCK_DISCARD))
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
	if (m_isRenderTexture)
	{
		IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(layer);
		if (pTextureForUpdate != NULL)
		{
			pTextureForUpdate->UnlockRect(layer);
			pTextureForUpdate->Release();
			pTextureForUpdate = NULL;
		}
	}
	else if (m_isDynamic || m_isManaged)
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
	if (m_autoGenMipmaps)
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
