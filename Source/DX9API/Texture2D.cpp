#include "Texture2D.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"

Texture2D::Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, bool recreateWhenDeviceLost)
	: m_texFormat(format)
	, m_usage(usage)
	, m_recreateWhenDeviceLost(recreateWhenDeviceLost)
	, m_pTexture(texture)
	, m_pTempTextureForUpdate(NULL)
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

IDirect3DTexture9 * Texture2D::GetD3DTexture()
{
	return m_pTexture;
}

inline IDirect3DTexture9 ** Texture2D::TextureForUpdate(unsigned int index)
{
	if (m_pTempTextureForUpdate.size() <= index)
		m_pTempTextureForUpdate.resize(index + 1);
	return &(m_pTempTextureForUpdate[index]);
}

RenderAPI::MappedResource Texture2D::LockRect(unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;

	if (m_usage == RenderAPI::RESUSAGE_Dynamic)
	{
		D3DLOCKED_RECT lockedRect;
		if (S_OK == m_pTexture->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]))
		{
			ret.Success = true;
			ret.DataPtr = lockedRect.pBits;
			ret.LinePitch = lockedRect.Pitch;
		}
	}
	else
	{
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
				if (S_OK == pTextureForUpdate->LockRect(layer, &lockedRect, NULL, s_lockOptions[lockOption]))
				{
					ret.Success = true;
					ret.DataPtr = lockedRect.pBits;
					ret.LinePitch = lockedRect.Pitch;
				}
			}
		}
	}
	return ret;
}

void Texture2D::UnlockRect(unsigned int layer)
{
	if (m_usage == RenderAPI::RESUSAGE_Dynamic)
	{
		m_pTexture->UnlockRect(layer);
	}
	else
	{
		IDirect3DTexture9*& pTextureForUpdate = *TextureForUpdate(layer);

		if (pTextureForUpdate != NULL)
		{
			pTextureForUpdate->UnlockRect(layer);

			IDirect3DDevice9* pDevice = NULL;
			HRESULT hr = m_pTexture->GetDevice(&pDevice);
			if (hr == S_OK)
			{
				pDevice->UpdateTexture(pTextureForUpdate, m_pTexture);
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

bool Texture2D::NeedRecreateWhenDeviceLost()
{
	return m_recreateWhenDeviceLost;
}
