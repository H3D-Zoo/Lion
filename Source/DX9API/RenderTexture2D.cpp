#include "Texture2D.h"
#include "EnumMapping.h"

RenderTexture2D::RenderTexture2D(IDirect3DTexture9 * texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage
	, unsigned int width, unsigned int height, IInternalLogger& logger)
	: Texture2D( texture, format, usage, false, width, height, false, logger)
	, m_pTempTextureForCopy(NULL)
{
}

RenderTexture2D::~RenderTexture2D()
{
	ReleaseCopiedSystemTexture();
}

RenderAPI::MappedResource RenderTexture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d, option=%d", layer, lockOption);

	RenderAPI::MappedResource ret;

	if (lockOption == RenderAPI::LOCK_NoOverWrite)
	{
		lockOption = RenderAPI::LOCK_Normal;
	}

	IDirect3DTexture9* pCopiedTexture = GetCopiedSystemTexture();
	if (pCopiedTexture != NULL)
	{
		D3DLOCKED_RECT lockedRect;
		HRESULT hr = pCopiedTexture->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]);
		if (hr == S_OK)
		{
			ret.DataPtr = lockedRect.pBits;
			ret.LinePitch = lockedRect.Pitch;
			ret.Success = true;
		}
		else
		{
			LOG_FUNCTION_FAILED_ERRCODE(&m_internalLogger, "Render Texture cannot be locked.", hr);
		}
	}
	else
	{
		LOG_FUNCTION_FAILED(&m_internalLogger, "Temporary Texture is not created.");
	}
	return ret;
}

void RenderTexture2D::UnlockRect(unsigned int layer)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "layer=%d", layer);

	if (m_pTempTextureForCopy != NULL)
	{
		m_pTempTextureForCopy->UnlockRect(layer);
		m_pTempTextureForCopy->Release();
		m_pTempTextureForCopy = NULL;
	}
	else
	{
		LOG_FUNCTION_FAILED(&m_internalLogger, "Temporary Texture is not created.");
	}
}

void RenderTexture2D::Resize(unsigned int width, unsigned int height)
{
	LOG_FUNCTION_PARAM(m_internalLogger, "width=%d, height=%d", width, height);

	ReleaseCopiedSystemTexture();
	Texture2D::Resize(width, height);
}


IDirect3DTexture9* RenderTexture2D::GetCopiedSystemTexture()
{
	IDirect3DDevice9* pDevice = NULL;
	m_pTexture->GetDevice(&pDevice);
	if (pDevice == NULL)
	{
		return NULL;
	}

	if (m_pTempTextureForCopy == NULL)
	{
		HRESULT creation = pDevice->CreateTexture(m_texWidth, m_texHeight, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &m_pTempTextureForCopy, NULL);
		if (FAILED(creation))
		{
			LOG_FUNCTION_FAILED_ERRCODE(&m_internalLogger, "Copy Texture cant be created.", creation);
			return NULL;
		}
	}

	if (m_pTempTextureForCopy != NULL)
	{
		IDirect3DSurface9* pSrcSurface = NULL, *pDstSurface = NULL;
		m_pTexture->GetSurfaceLevel(0, &pSrcSurface);
		m_pTempTextureForCopy->GetSurfaceLevel(0, &pDstSurface);

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
	return m_pTempTextureForCopy;
}

void RenderTexture2D::ReleaseCopiedSystemTexture()
{
	if (m_pTempTextureForCopy != NULL)
	{
		m_pTempTextureForCopy->Release();
		m_pTempTextureForCopy = NULL;
	}
}