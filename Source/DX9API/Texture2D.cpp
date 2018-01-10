#include <stdlib.h>
#include "Texture2D.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"

Texture2D::Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged,
	unsigned int width, unsigned int height, bool autoGenMipmaps, IInternalLogger& logger)
	: m_internalLogger(logger)
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
	
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
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
	LOG_FUNCTION_CALL(m_internalLogger);
	m_texWidth = width;
	m_texHeight = height;
}

unsigned int Texture2D::AddReference()
{
	return ++m_refCount;
}

IDirect3DTexture9 * Texture2D::GetD3DTexture()
{
	return m_pTexture;
}

RenderAPI::MappedResource Texture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d, lock option=%d", layer, lockOption);

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
		LOG_FUNCTION_FAILED_ERRCODE(&m_internalLogger, "LockRect Failed.", hr);
		ret.Success = false;
	}

	return ret;
}

void Texture2D::UnlockRect(unsigned int layer)
{
	LOG_FUNCTION_CALL(m_internalLogger);
	m_pTexture->UnlockRect(layer);
}

void Texture2D::SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "filter=%d", filter);

	m_pTexture->SetAutoGenFilterType(s_d3dSamplerFilter[filter]);
}

void Texture2D::GenerateMipmaps()
{
	LOG_FUNCTION_CALL(m_internalLogger);
	if (!m_autoGenMipmaps)
	{
		m_pTexture->GenerateMipSubLevels();
	}
}

RenderAPI::TextureSurface* Texture2D::GetSurface(unsigned int index)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "index=%d", index);

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
		IDirect3DSurface9* pSurface = NULL;
		if (S_OK == m_pTexture->GetSurfaceLevel(index, &pSurface))
		{
			m_surfaces[index] = new TextureSurface(m_internalLogger, this, pSurface);
		}
	}

	if (m_surfaces[index])
	{
		m_surfaces[index]->AddReference();
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
	LOG_FUNCTION_CALL(m_internalLogger);

	if (format == RenderAPI::IMG_ERR)
		return false;

	return S_OK == D3DXSaveTextureToFileA(fileName, s_d3dxFileFormat[format], NULL, NULL);
}

TextureSurface::TextureSurface(IInternalLogger& logger, Texture2D* pTexture, IDirect3DSurface9 * pSurface)
	: m_pParentTexture(pTexture)
	, m_internalLogger(logger)
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
	LOG_FUNCTION_CALL(m_internalLogger);
	if (m_hDC == NULL)
	{
		m_pSurface->GetDC(&m_hDC);
	}
	return m_hDC;
}

void TextureSurface::ReleaseDC()
{
	LOG_FUNCTION_CALL(m_internalLogger);
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
	LOG_FUNCTION_CALL(m_internalLogger);

	if (0 == --m_refCount)
	{
		delete this;
	}
	else
	{
		if (m_pParentTexture != NULL)
		{
			m_pParentTexture->Release();
		}
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

IDirect3DSurface9 * TextureSurface::GetD3DTextureSurfacePtr()
{
	return m_pSurface;
}

RenderSurface2D::RenderSurface2D(IDirect3DSurface9* pSurface, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, IInternalLogger& logger)
	: TextureSurface(logger, NULL, pSurface)
	, m_internalLogger(logger)
	, m_texFormat(format)
	, m_texWidth(width)
	, m_texHeight(height)
	, m_pTempTextureForCopy(format, width, height, 1)
{
}

RenderSurface2D::~RenderSurface2D()
{
	m_pTempTextureForCopy.ReleaseTexture();
}


RenderAPI::MappedResource RenderSurface2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption) 
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d, lock option=%d", layer, lockOption);

	if (lockOption == RenderAPI::LOCK_NoOverWrite)
	{
		lockOption = RenderAPI::LOCK_Normal;
	}

	if (CopyToSystemTexture())
	{
		return m_pTempTextureForCopy.Lock(layer, lockOption);
	}
	else
	{
		return RenderAPI::MappedResource();
	}
}

void RenderSurface2D::UnlockRect(unsigned int layer) 
{
	LOG_FUNCTION_CALL(m_internalLogger);
	m_pTempTextureForCopy.Unlock(layer);
	if (!m_pTempTextureForCopy.IsSomeLayerLocking())
	{
		m_pTempTextureForCopy.ReleaseTexture();
	}
}

RenderAPI::TextureSurface * RenderSurface2D::GetSurface(unsigned int layer)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d", layer);
	if (layer == 0)
	{
		AddReference();
		return this;
	}
	else
	{
		return NULL;
	}
}


bool RenderSurface2D::CopyToSystemTexture()
{
	IDirect3DSurface9* pSrcSurface = GetD3DTextureSurfacePtr();
	IDirect3DDevice9* pDevice = NULL;
	pSrcSurface->GetDevice(&pDevice);
	if (pDevice == NULL)
	{
		return false;
	}

	if (!m_pTempTextureForCopy.IsCreated() &&
		!m_pTempTextureForCopy.Create(pDevice))
	{
		pDevice->Release();
		return false;
	}

	IDirect3DSurface9* pDstSurface = NULL;
	HRESULT hrGetDstSurface = m_pTempTextureForCopy.GetTexturePtr()->GetSurfaceLevel(0, &pDstSurface);

	if (hrGetDstSurface == S_OK)
	{
		HRESULT hr = pDevice->GetRenderTargetData(pSrcSurface, pDstSurface);
		pDstSurface->Release();
		pDevice->Release();
		return hr == S_OK;
	}
	else
	{
		pDevice->Release();
		return false;
	}
}

TemporaryTexture::TemporaryTexture(RenderAPI::TextureFormat f, unsigned int w, unsigned int h, unsigned int layerCount)
	: m_pTexture(NULL)
	, m_lockLayerBits(0)
	, m_texFormat(f)
	, m_texLayers(layerCount)
	, m_texWidth(w)
	, m_texHeight(h)
{

}

void TemporaryTexture::SetLayerLocking(unsigned int layer, bool locked)
{
	unsigned int layerMask = 1 << layer;
	if (locked)
	{
		m_lockLayerBits |= layerMask;
	}
	else
	{
		m_lockLayerBits &= ~layerMask;
	}
}

bool TemporaryTexture::IsLayerLocking(unsigned int layer) const
{
	unsigned int layerMask = 1 << layer;
	return (m_lockLayerBits & layerMask) > 0;
}

bool TemporaryTexture::Create(IDirect3DDevice9* pDevice)
{
	if (m_pTexture == NULL)
	{
		HRESULT creation = pDevice->CreateTexture(m_texWidth, m_texHeight, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &m_pTexture, NULL);
		if (FAILED(creation))
		{
			m_pTexture = NULL;
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}


RenderAPI::MappedResource TemporaryTexture::Lock(unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;
	if (layer < m_texLayers && !IsLayerLocking(layer))
	{
		D3DLOCKED_RECT lockedRect;
		if (S_OK == m_pTexture->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]))
		{
			ret.DataPtr = lockedRect.pBits;
			ret.LinePitch = lockedRect.Pitch;
			ret.Success = true;
			SetLayerLocking(layer, true);
		}
		else
		{
			ret.Success = false;
		}
	}
	else
	{
		ret.Success = false;
	}
	return ret;
}

bool TemporaryTexture::Unlock(unsigned int layer)
{
	if (IsLayerLocking(layer))
	{
		SetLayerLocking(layer, false);
		if (m_pTexture != NULL)
		{
			HRESULT hr = m_pTexture->UnlockRect(layer);
			return (S_OK == hr);
		}
	}
	return false;
}

void  TemporaryTexture::Resize(unsigned int w, unsigned int h, unsigned int layerCount)
{
	if (m_texWidth != w || m_texHeight != h)
	{
		ReleaseTexture();
		m_texWidth = w;
		m_texHeight = h;
		m_texLayers = layerCount;
	}
}

void TemporaryTexture::ReleaseTexture()
{
	if (m_pTexture == NULL)
	{
		if (IsSomeLayerLocking())
		{
			for (int i = 0; i < m_texLayers; i++)
			{
				if (IsLayerLocking(i))
				{
					Unlock(i);
				}
			}
			m_lockLayerBits = 0;
		}
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}