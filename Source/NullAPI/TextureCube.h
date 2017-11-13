#pragma once
#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "APIInstance.h"
#include "RefCount.hpp"

class RenderTarget;
class DepthStencil;

class TextureCube : public RenderAPI::TextureCube
{
public:
	TextureCube(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int edgeLength, bool autoGenMipmaps);
	
	virtual RenderAPI::TextureFormat GetFormat() const;

	virtual unsigned int GetLength() const;
	
	virtual void SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter);

	virtual void GenerateMipmaps();

	virtual bool AutoGenMipmaps() const;

	virtual bool IsCubemap() const;

	virtual bool NeedRecreateWhenDeviceLost() const;

	virtual unsigned int GetEdgeLength() const;

	virtual RenderAPI::MappedResource LockRect(RenderAPI::CubemapFace face, unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(RenderAPI::CubemapFace face, unsigned int layer);

	virtual unsigned int AddReference();

	virtual void Release();

	unsigned int ClearStamp;

private:
	RefCount m_refCount;
	const RenderAPI::ResourceUsage m_usage;
	const RenderAPI::TextureFormat m_texFormat;
	const bool m_autoGenMipmaps;
	const bool m_isManaged;
	const bool m_isDynamic;
	const unsigned int m_texEdgeLength;

	std::vector<char> m_faceBuffer;
};
