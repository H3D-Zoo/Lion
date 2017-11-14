#include "Texture2D.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"
#include "APIInstance.h"
#include "Context.h"

NoLockableTexture2D::NoLockableTexture2D(IDirect3DTexture9 * texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage,
	bool isManaged, unsigned int width, unsigned int height, bool autoGenMipmaps, IInternalLogger& logger)
	: Texture2D(texture, format, usage, isManaged, width, height, autoGenMipmaps, logger)
	, m_pTempTextureForUpdate(NULL)
	, m_lockLayerBits(0)
{
}

NoLockableTexture2D::~NoLockableTexture2D()
{
	if (m_pTempTextureForUpdate != NULL)
	{
		m_pTempTextureForUpdate->Release();
		m_pTempTextureForUpdate = NULL;
	}
}

RenderAPI::MappedResource NoLockableTexture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	//if (m_isDynamic || m_isManaged)

	//Textures created with D3DPOOL_DEFAULT are not lockable.
	//Textures created in video memory are lockable when created with USAGE_DYNAMIC.
	RenderAPI::MappedResource ret;

	if (IsLayerLocking(layer))
	{
		return ret;
	}

	if (lockOption == RenderAPI::LOCK_NoOverWrite)
	{
		lockOption = RenderAPI::LOCK_Normal;
	}
	
	IDirect3DDevice9* pDevice = NULL;
	HRESULT hr = m_pTexture->GetDevice(&pDevice);
	if (hr == S_OK)
	{
		if (m_pTempTextureForUpdate == NULL)
		{
			hr = pDevice->CreateTexture(m_texWidth, m_texHeight, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &m_pTempTextureForUpdate, NULL);
		}

		if (hr == S_OK)
		{
			D3DLOCKED_RECT lockedRect;
			if (S_OK == m_pTempTextureForUpdate->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]))
			{
				ret.DataPtr = lockedRect.pBits;
				ret.LinePitch = lockedRect.Pitch;
				ret.Success = true;
				SetLayerLocking(layer, true);
			}
			else
			{
				m_internalLogger.LogError("Texture2D::Lock", "helper texture lock failed.", hr);
				ret.Success = false;
			}
		}
		else
		{
			m_internalLogger.LogError("Texture2D::Lock", "helper texture creation failed.", hr);
			ret.Success = false;
		}
	}
	return ret;
}

void NoLockableTexture2D::UnlockRect(unsigned int layer)
{
	if (IsLayerLocking(layer))
	{
		SetLayerLocking(layer, false);
		if (m_pTempTextureForUpdate != NULL)
		{
			HRESULT hr = m_pTempTextureForUpdate->UnlockRect(layer);
			if (S_OK == hr)
			{
				IDirect3DSurface9 *pSurfaceSrc, *pSurfaceDst;
				IDirect3DDevice9* pDevice;
				m_pTexture->GetDevice(&pDevice);
				m_pTempTextureForUpdate->GetSurfaceLevel(layer, &pSurfaceSrc);
				m_pTexture->GetSurfaceLevel(layer, &pSurfaceDst);
				hr = pDevice->UpdateSurface(pSurfaceSrc, NULL, pSurfaceDst, NULL);
				if (hr != S_OK)
				{
					m_internalLogger.LogError("Texture2D::UnlockRect", "helper texture update failed.", hr);
				}
				pSurfaceSrc->Release();
				pSurfaceDst->Release();

			}

			if (!IsSomeLayerLocking())
			{
				m_pTempTextureForUpdate->Release();
				m_pTempTextureForUpdate = NULL;
			}
		}
	}
}

void NoLockableTexture2D::GenerateMipmaps()
{
	unsigned int oldCount = GetLayerCount();
	if (m_pTempTextureForUpdate != NULL)
	{
		for (unsigned int i = 0, n = oldCount; i < n; i++)
		{
			if (IsLayerLocking(i))
			{
				m_pTempTextureForUpdate->UnlockRect(i);
			}
		}
		m_pTempTextureForUpdate->Release();
		m_pTempTextureForUpdate = NULL;
	}

	Texture2D::GenerateMipmaps();
	m_lockLayerBits = 0;
}

void NoLockableTexture2D::SetLayerLocking(unsigned int layer, bool locked)
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

bool NoLockableTexture2D::IsLayerLocking(unsigned int layer) const
{
	unsigned int layerMask = 1 << layer;
	return (m_lockLayerBits & layerMask) > 0;
}

bool NoLockableTexture2D::IsSomeLayerLocking() const
{
	return m_lockLayerBits > 0;
}
