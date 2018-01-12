#include <stdlib.h>
#include "TextureCube.h"
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

TextureCube::TextureCube(IDirect3DCubeTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged, unsigned int edgeLength, bool autoGenMipmaps, IInternalLogger& logger)
	: m_internalLogger(logger)
	, m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_isManaged(isManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_pTexture(texture)
	, m_texEdgeLength(edgeLength)
	, ClearStamp(0)
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

unsigned int TextureCube::GetLength() const
{
	return 6 * GetTextureLength(m_texEdgeLength, m_texEdgeLength, m_texFormat, m_autoGenMipmaps);
}

unsigned int TextureCube::GetEdgeLength() const
{
	return m_texEdgeLength;
}

void TextureCube::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

IDirect3DCubeTexture9 * TextureCube::GetD3DTexture()
{
	return m_pTexture;
}

RenderAPI::MappedResource TextureCube::LockRect(RenderAPI::CubemapFace face, unsigned int layer, RenderAPI::LockOption lockOption)
{
	LOG_FUNCTION_D(m_internalLogger, "object=%X, face=%d, layer=%d, lock option=%d", this, face, layer, lockOption);

	RenderAPI::MappedResource ret;
	
	if (lockOption == RenderAPI::LOCK_NoOverWrite)
	{
		lockOption = RenderAPI::LOCK_Normal;
		LOG_FUNCTION_V(m_internalLogger, "change lock option to normal");
	}

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
			LOG_FUNCTION_W(m_internalLogger, "LockRect Failed", hr);
		}
	}
	else
	{
		ret.Success = false;
		LOG_FUNCTION_W(m_internalLogger, "only dynamic/Managed texture can be locked.");
	}
	return ret;
}

void TextureCube::UnlockRect(RenderAPI::CubemapFace face, unsigned int layer)
{
	LOG_FUNCTION_D(m_internalLogger, "object=%X, face=%d, layer=%d", this, face, layer);
	m_pTexture->UnlockRect(s_d3dCubeFaces[face], layer);
}

unsigned int TextureCube::AddReference()
{
	return ++m_refCount;
}

void TextureCube::SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter)
{
	m_pTexture->SetAutoGenFilterType(s_d3dSamplerFilter[filter]);
}

void TextureCube::GenerateMipmaps()
{
	LOG_FUNCTION_CALL(m_internalLogger, LOG_Verbose);

	if (!m_autoGenMipmaps)
	{
		m_pTexture->GenerateMipSubLevels();
	}
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