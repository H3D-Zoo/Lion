#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "APIInstance.h"
#include "DX9Include.h"

class RenderTarget;
class DepthStencil;

class TextureCube : public RenderAPI::TextureCube
{
public:
	TextureCube(APIInstance* pAPIInstance, IDirect3DCubeTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int edgeLength, bool autoGenMipmaps, bool recreateWhenDeviceLost);

	~TextureCube();

	virtual RenderAPI::TextureFormat GetFormat() const;

	virtual void GenerateMipmaps();

	virtual bool AutoGenMipmaps() const;

	virtual bool IsCubemap() const;

	virtual bool NeedRecreateWhenDeviceLost() const;

	virtual unsigned int GetEdgeLength() const;

	virtual RenderAPI::MappedResource LockRect(RenderAPI::CubemapFace face, unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(RenderAPI::CubemapFace face, unsigned int layer);

	virtual void Release();

	IDirect3DCubeTexture9* GetD3DTexture();

private:
	APIInstance* m_pAPIInstance;
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::TextureFormat m_texFormat;
	const bool m_autoGenMipmaps;
	const bool m_recreateWhenDeviceLost;
	IDirect3DCubeTexture9* m_pTexture;
	unsigned int m_texEdgeLength;
};
