#include "TextureCube.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "EnumMapping.h"

namespace
{
	D3DCUBEMAP_FACES s_d3dCubeFaces[] =
	{
		 D3DCUBEMAP_FACE_POSITIVE_X,
		 D3DCUBEMAP_FACE_NEGATIVE_X,
		 D3DCUBEMAP_FACE_POSITIVE_Y,
		 D3DCUBEMAP_FACE_NEGATIVE_Y,
		 D3DCUBEMAP_FACE_POSITIVE_Z,
		 D3DCUBEMAP_FACE_NEGATIVE_Z,
	};
}

TextureCube::TextureCube(IDirect3DCubeTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int edgeLength, bool recreateWhenDeviceLost)
	: m_texFormat(format)
	, m_usage(usage)
	, m_recreateWhenDeviceLost(recreateWhenDeviceLost)
	, m_pTexture(texture)
	, m_pTempTextureForUpdate(NULL)
	, m_texEdgeLength(edgeLength)
{

}

TextureCube::~TextureCube()
{
	m_pTexture->Release();
	m_pTexture = NULL;
}

RenderAPI::TextureFormat TextureCube::GetFormat() const
{
	return m_texFormat;
}

unsigned int TextureCube::GetEdgeLength() const
{
	return m_texEdgeLength;
}

void TextureCube::Release()
{
	delete this;
}

IDirect3DCubeTexture9 * TextureCube::GetD3DTexture()
{
	return m_pTexture;
}

IDirect3DCubeTexture9 ** TextureCube::TextureForUpdate(unsigned int index)
{
	if (m_pTempTextureForUpdate.size() <= index)
		m_pTempTextureForUpdate.resize(index + 1);
	return &(m_pTempTextureForUpdate[index]);
}

RenderAPI::MappedResource TextureCube::LockRect(RenderAPI::CubemapFace face, unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;

	if (m_usage == RenderAPI::RESUSAGE_Dynamic)
	{
		D3DLOCKED_RECT lockedRect;
		if (S_OK == m_pTexture->LockRect(s_d3dCubeFaces[face], layer, &lockedRect, NULL, s_lockOptions[lockOption]))
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
			IDirect3DCubeTexture9*& pTextureForUpdate = *TextureForUpdate(layer);
			if (pTextureForUpdate != NULL)
			{
				pTextureForUpdate->Release();
			}

			hr = pDevice->CreateCubeTexture(m_texEdgeLength, 0, D3DUSAGE_DYNAMIC, s_TextureFormats[m_texFormat], D3DPOOL_SYSTEMMEM, &pTextureForUpdate, NULL);
			if (hr == S_OK)
			{
				D3DLOCKED_RECT lockedRect;
				if (S_OK == pTextureForUpdate->LockRect(s_d3dCubeFaces[face], layer, &lockedRect, NULL, s_lockOptions[lockOption]))
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

void TextureCube::UnlockRect(RenderAPI::CubemapFace face, unsigned int layer)
{
	if (m_usage == RenderAPI::RESUSAGE_Dynamic)
	{
		m_pTexture->UnlockRect(s_d3dCubeFaces[face], layer);
	}
	else
	{
		IDirect3DCubeTexture9*& pTextureForUpdate = *TextureForUpdate(layer);

		if (pTextureForUpdate != NULL)
		{
			pTextureForUpdate->UnlockRect(s_d3dCubeFaces[face], layer);

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

void TextureCube::GenerateMipmaps()
{
	m_pTexture->GenerateMipSubLevels();
}

bool TextureCube::NeedRecreateWhenDeviceLost() const
{
	return m_recreateWhenDeviceLost;
}

bool TextureCube::IsCubemap() const
{
	return true;
}