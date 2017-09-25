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

TextureCube::TextureCube(APIInstance* pAPIInstance, IDirect3DCubeTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged, unsigned int edgeLength, bool autoGenMipmaps)
	: m_pAPIInstance(pAPIInstance)
	, m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_isManaged(isManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
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
	if (m_isDynamic || m_isManaged)
	{
		D3DLOCKED_RECT lockedRect;
		HRESULT hr = m_pTexture->LockRect(s_d3dCubeFaces[face], layer, &lockedRect, NULL, s_lockOptions[lockOption]);
		if (S_OK == hr)
		{
			ret.Success = true;
			ret.DataPtr = lockedRect.pBits;
			ret.LinePitch = lockedRect.Pitch;
		}
		else
		{
			ret.Success = false;
			m_pAPIInstance->LogError("TextureCube::LockRect", "Lock failed.", hr);
		}
	}
	else
	{
		ret.Success = false;
		m_pAPIInstance->LogError("TextureCube::LockRect", "Only Dynamic or Managed Texture can be loked.");
	}
	return ret;
}

void TextureCube::UnlockRect(RenderAPI::CubemapFace face, unsigned int layer)
{
	m_pTexture->UnlockRect(s_d3dCubeFaces[face], layer);
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
	return !m_isManaged;
}

bool TextureCube::IsCubemap() const
{
	return true;
}