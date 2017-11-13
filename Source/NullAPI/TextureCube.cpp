#include "TextureCube.h"
#include <stdlib.h>
#include "RenderTarget.h"
#include "DepthStencil.h"

TextureCube::TextureCube(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int edgeLength, bool autoGenMipmaps)
	: m_texFormat(format)
	, m_usage(usage)
	, m_autoGenMipmaps(autoGenMipmaps)
	, m_isManaged(usage == RenderAPI::RESUSAGE_DynamicManaged || usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_StaticWOManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_texEdgeLength(edgeLength)
	, ClearStamp(0)
{
	m_faceBuffer.resize(GetTextureLength(m_texEdgeLength, m_texEdgeLength, m_texFormat, m_autoGenMipmaps));
}

RenderAPI::TextureFormat TextureCube::GetFormat() const
{
	return m_texFormat;
}

unsigned int TextureCube::GetLength() const
{
	return 6 * static_cast<unsigned int>(m_faceBuffer.size());
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

RenderAPI::MappedResource TextureCube::LockRect(RenderAPI::CubemapFace face, unsigned int layer, RenderAPI::LockOption lockOption)
{
	RenderAPI::MappedResource ret;
	
	if (lockOption == RenderAPI::LOCK_NoOverWrite)
	{
		lockOption = RenderAPI::LOCK_Normal;
	}

	if (m_isDynamic || m_isManaged)
	{
		ret.Success = true;
		ret.DataPtr = &(m_faceBuffer[0]);
		ret.LinePitch = static_cast<unsigned int>(m_faceBuffer.size()) / m_texEdgeLength;
	}
	return ret;
}

void TextureCube::UnlockRect(RenderAPI::CubemapFace face, unsigned int layer)
{

}

unsigned int TextureCube::AddReference()
{
	return ++m_refCount;
}

void TextureCube::SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter)
{

}

void TextureCube::GenerateMipmaps()
{

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