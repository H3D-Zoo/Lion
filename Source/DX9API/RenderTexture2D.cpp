#include "Texture2D.h"
#include "EnumMapping.h"

RenderTexture2D::RenderTexture2D(IDirect3DTexture9 * texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage
	, unsigned int width, unsigned int height, IInternalLogger& logger)
	: Texture2D(texture, format, usage, false, width, height, false, logger)
	, m_pTempTextureForCopy(format, width, height, texture->GetLevelCount())
{
}

RenderTexture2D::~RenderTexture2D()
{
	m_pTempTextureForCopy.ReleaseTexture();
}

RenderAPI::MappedResource RenderTexture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d, option=%d", layer, lockOption);

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

void RenderTexture2D::UnlockRect(unsigned int layer)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d", layer);

	m_pTempTextureForCopy.Unlock(layer);
	if (!m_pTempTextureForCopy.IsSomeLayerLocking())
	{
		m_pTempTextureForCopy.ReleaseTexture();
	}
}

void RenderTexture2D::Resize(unsigned int width, unsigned int height)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "width=%d, height=%d", width, height);

	m_pTempTextureForCopy.Resize(width, height, m_pTexture->GetLevelCount());
	Texture2D::Resize(width, height);
}

bool RenderTexture2D::CopyToSystemTexture()
{
	IDirect3DDevice9* pDevice = NULL;
	m_pTexture->GetDevice(&pDevice);
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

	IDirect3DSurface9* pSrcSurface = NULL;
	IDirect3DSurface9* pDstSurface = NULL;

	HRESULT hrGetSrcSurface = m_pTexture->GetSurfaceLevel(0, &pSrcSurface);
	HRESULT hrGetDstSurface = m_pTempTextureForCopy.GetTexturePtr()->GetSurfaceLevel(0, &pDstSurface);

	if (hrGetSrcSurface == S_OK && hrGetDstSurface == S_OK)
	{
		HRESULT hr = pDevice->GetRenderTargetData(pSrcSurface, pDstSurface);
		pSrcSurface->Release();
		pDstSurface->Release();
		pDevice->Release();
		return hr == S_OK;
	}
	else
	{
		if (pSrcSurface != NULL) pSrcSurface->Release();
		if (pDstSurface != NULL) pDstSurface->Release();
		pDevice->Release();
		return false;
	}
}