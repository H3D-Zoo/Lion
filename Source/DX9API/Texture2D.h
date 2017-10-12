#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "RefCount.hpp"
#include "DX9Include.h"

class RenderTarget;
class DepthStencil;
class Texture2D;

class TextureSurface : public RenderAPI::TextureSurface
{
public:
	TextureSurface(Texture2D* pTexture, IDirect3DSurface9* pSurface);

	~TextureSurface();

	virtual void* GetDC();

	virtual void ReleaseDC();

	virtual void Release();;

	void AddRef();

	IDirect3DSurface9* GetD3DTextureSurfacePtr();

private:
	RefCount m_refCount;
	Texture2D* m_pParentTexture;
	IDirect3DSurface9* m_pSurface;
	HDC m_hDC;
};

class Texture2D : public RenderAPI::Texture2D
{
public:
	Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage,
		unsigned int width, unsigned int height, 
		bool autoGenMipmaps, bool recreateWhenDeviceLost, bool isRenderTexture);

	~Texture2D();

	virtual RenderAPI::TextureFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void GenerateMipmaps();
	
	RenderAPI::TextureSurface* GetSurface(unsigned int index);

	virtual unsigned int GetLayerCount() const;

	virtual bool NeedRecreateWhenDeviceLost() const;

	virtual bool AutoGenMipmaps() const;

	virtual bool IsCubemap() const;

	virtual bool IsRenderTexture() const;

	virtual bool SaveToFile(const char* fileName, RenderAPI::ImageFormat format);

	virtual void Release();

	void Resize(unsigned int width, unsigned int height);

	void AddRef();

	IDirect3DTexture9* GetD3DTexture();

private:
	IDirect3DTexture9** TextureForUpdate(unsigned int index);

	RefCount m_refCount;
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::TextureFormat m_texFormat;
	const bool m_autoGenMipmaps;
	const bool m_recreateWhenDeviceLost;
	const bool m_isRenderTexture;
	IDirect3DTexture9* m_pTexture;
	std::vector<TextureSurface*> m_surfaces;
	std::vector<IDirect3DTexture9*> m_pTempTextureForUpdate;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
};
