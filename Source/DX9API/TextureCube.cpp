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

TextureCube::TextureCube(APIInstance* pAPIInstance, IDirect3DCubeTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int edgeLength, bool autoGenMipmaps, bool recreateWhenDeviceLost)
	: m_pAPIInstance(pAPIInstance)
	, m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_recreateWhenDeviceLost(recreateWhenDeviceLost)
	, m_pTexture(texture)
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

RenderAPI::MappedResource TextureCube::LockRect(RenderAPI::CubemapFace face, unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;

	if (m_usage == RenderAPI::RESUSAGE_Immuable)
	{
		ret.Success = false;
	}
	else
	{
		D3DLOCKED_RECT lockedRect;
		HRESULT hr = m_pTexture->LockRect(s_d3dCubeFaces[face], layer, &lockedRect, NULL, GetLockOption(lockOption, m_usage));
		if (S_OK == hr)
		{
			ret.Success = true;
			ret.DataPtr = lockedRect.pBits;
			ret.LinePitch = lockedRect.Pitch;
		}
		else
		{
			ret.Success = false;
			m_pAPIInstance->LogError("TextureCube::UnlockRect", "Lock failed.", hr);
		}
	}
	return ret;
}

void TextureCube::UnlockRect(RenderAPI::CubemapFace face, unsigned int layer)
{
	if (m_usage == RenderAPI::RESUSAGE_Immuable)
	{
		m_pAPIInstance->LogError("TextureCube::UnlockRect", "Immuable Texture cannot be locked.");
	}
	else
	{
		m_pTexture->UnlockRect(s_d3dCubeFaces[face], layer);
	}
}

void TextureCube::GenerateMipmaps()
{
	m_pTexture->GenerateMipSubLevels();
}

bool TextureCube::AutoGenMipmaps() const 
{
	return m_autoGenMipmaps;
}

bool TextureCube::NeedRecreateWhenDeviceLost() const
{
	return m_recreateWhenDeviceLost;
}

bool TextureCube::IsCubemap() const
{
	return true;
}