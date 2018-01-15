#include "Texture2D.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"
#include "APIInstance.h"
#include "Context.h"

NoLockableTexture2D::NoLockableTexture2D(IDirect3DTexture9 * texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage,
	bool isManaged, unsigned int width, unsigned int height, bool autoGenMipmaps, RenderAPI::Logger& logger)
	: Texture2D(texture, format, usage, isManaged, width, height, autoGenMipmaps, logger)
	, m_pTempTextureForUpdate(format, width, height, texture->GetLevelCount(), logger)
{
}

NoLockableTexture2D::~NoLockableTexture2D()
{
	m_pTempTextureForUpdate.ReleaseTexture();
}

RenderAPI::MappedResource NoLockableTexture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	LOG_FUNCTION_V(m_internalLogger, "object=%X, layer=%d, lock option=%d", this, layer, lockOption);
	//if (m_isDynamic || m_isManaged)

	//Textures created with D3DPOOL_DEFAULT are not lockable.
	//Textures created in video memory are lockable when created with USAGE_DYNAMIC.

	if (lockOption == RenderAPI::LOCK_NoOverWrite)
	{
		lockOption = RenderAPI::LOCK_Normal;
		LOG_FUNCTION_V(m_internalLogger, "change lock option to normal");
	}

	if (!m_pTempTextureForUpdate.IsCreated())
	{
		IDirect3DDevice9* pDevice = NULL;
		HRESULT hr = m_pTexture->GetDevice(&pDevice);

		if (hr == S_OK)
		{
			m_pTempTextureForUpdate.Create(pDevice);
			pDevice->Release();
		}
		else
		{
			LOG_FUNCTION_E(m_internalLogger, "cannot retrieve device form texture. error=%X", hr);
		}

		if (!m_pTempTextureForUpdate.IsCreated())
		{
			LOG_FUNCTION_E(m_internalLogger, "helper texture creation failed.", hr);
		}
	}
	
	return m_pTempTextureForUpdate.Lock(layer, lockOption);
}

void NoLockableTexture2D::UnlockRect(unsigned int layer)
{
	LOG_FUNCTION_V(m_internalLogger, "object=%X, layer=%d", this, layer);

	if (m_pTempTextureForUpdate.Unlock(layer))
	{
		IDirect3DDevice9* pDevice;
		HRESULT hr = m_pTexture->GetDevice(&pDevice);
		if (S_OK == hr)
		{
			IDirect3DSurface9 *pSurfaceSrc = NULL, *pSurfaceDst = NULL;
			HRESULT hrGetSrcSurface = m_pTempTextureForUpdate.GetTexturePtr()->GetSurfaceLevel(layer, &pSurfaceSrc);
			HRESULT hrGetDstSurface = m_pTexture->GetSurfaceLevel(layer, &pSurfaceDst);
			if (S_OK == hrGetSrcSurface && S_OK == hrGetDstSurface)
			{
				HRESULT hr = pDevice->UpdateSurface(pSurfaceSrc, NULL, pSurfaceDst, NULL);
				if (hr != S_OK)
				{
					LOG_FUNCTION_E(m_internalLogger, "helper texture update failed.", hr);
				}
			}
			else
			{
				LOG_FUNCTION_E(m_internalLogger, "cannot retrieve surfaces, src=%d, dst=%d", hrGetSrcSurface, hrGetDstSurface);
			}
			if (pSurfaceSrc) pSurfaceSrc->Release();
			if (pSurfaceDst) pSurfaceDst->Release();
			pDevice->Release();
		}
		else
		{
			LOG_FUNCTION_E(m_internalLogger, "cannot retrieve device from texture. error=%X", hr);
		}
	}
}

void NoLockableTexture2D::GenerateMipmaps()
{
	m_pTempTextureForUpdate.ReleaseTexture();
	Texture2D::GenerateMipmaps();
}