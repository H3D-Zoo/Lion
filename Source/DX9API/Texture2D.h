#pragma once
#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "APIInstance.h"
#include "RefCount.hpp"
#include "DX9Include.h"

class RenderTarget;
class DepthStencil;
class Texture2D;

class TextureSurface : public RenderAPI::TextureSurface
{
public:
	TextureSurface(IInternalLogger&, ::Texture2D*, IDirect3DSurface9*);

	~TextureSurface();

	virtual void* GetDC();

	virtual void ReleaseDC();

	virtual bool SaveToFile(const char* fileName, RenderAPI::ImageFormat format);

	virtual unsigned int AddReference();

	virtual void Release();

	IDirect3DSurface9* GetD3DTextureSurfacePtr();

private:
	RefCount m_refCount;
	IInternalLogger& m_internalLogger;
	::Texture2D* m_pParentTexture;
	IDirect3DSurface9* m_pSurface;
	HDC m_hDC;
};

class Texture2D : public RenderAPI::Texture2D
{
public:
	Texture2D(IDirect3DTexture9*, RenderAPI::TextureFormat, RenderAPI::ResourceUsage, bool isManaged,
		unsigned int width, unsigned int height, bool autoGenMipmaps, IInternalLogger&);

	~Texture2D();

	virtual RenderAPI::TextureFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual unsigned int GetLength() const;

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter);

	virtual void GenerateMipmaps();
	
	virtual RenderAPI::TextureSurface* GetSurface(unsigned int index);

	virtual unsigned int GetLayerCount() const;

	virtual bool NeedRecreateWhenDeviceLost() const;

	virtual bool AutoGenMipmaps() const;

	virtual bool IsCubemap() const { return false; }

	virtual bool IsRenderTexture() const { return false; }

	virtual bool SaveToFile(const char* fileName, RenderAPI::ImageFormat format);

	virtual void Release();

	virtual void Resize(unsigned int width, unsigned int height);

	virtual unsigned int AddReference();

	IDirect3DTexture9* GetD3DTexture();

	unsigned int ClearStamp;

protected:
	IInternalLogger& m_internalLogger;
	IDirect3DTexture9* m_pTexture;
	const bool m_autoGenMipmaps;
	const bool m_isManaged;
	const bool m_isDynamic;
	const RenderAPI::ResourceUsage m_usage;
	const RenderAPI::TextureFormat m_texFormat;

	unsigned int m_texWidth;
	unsigned int m_texHeight;
private:
	RefCount m_refCount;
	std::vector<TextureSurface*> m_surfaces;
};

class TemporaryTexture
{
	IInternalLogger& m_logger;
	RenderAPI::TextureFormat m_texFormat;
	unsigned int m_texLayers;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
	IDirect3DTexture9* m_pTexture;
	unsigned int m_lockLayerBits;
	void SetLayerLocking(unsigned int layer, bool locked);
	bool IsLayerLocking(unsigned int layer) const;
public:
	TemporaryTexture(IInternalLogger& logger, RenderAPI::TextureFormat f, unsigned int w, unsigned int h, unsigned int layerCount);
	bool Create(IDirect3DDevice9* pDevice);
	IDirect3DTexture9* GetTexturePtr() const { return m_pTexture; }
	bool IsCreated() const { return m_pTexture != NULL; }
	bool IsSomeLayerLocking() const{ return m_lockLayerBits > 0; }
	RenderAPI::MappedResource Lock(unsigned int layer, RenderAPI::LockOption lockOption);
	bool Unlock(unsigned int layer);
	void UnlockAll();
	void Resize(unsigned int w, unsigned int h, unsigned int layerCount);
	void ReleaseTexture();
};

class RenderTexture2D : public Texture2D
{
public:
	RenderTexture2D(IDirect3DTexture9*, RenderAPI::TextureFormat, RenderAPI::ResourceUsage usage,
		unsigned int width, unsigned int height, IInternalLogger&);

	~RenderTexture2D();

	virtual bool IsRenderTexture() const { return true; }

	virtual bool NeedRecreateWhenDeviceLost() const { return false; }

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter) { }

	virtual void GenerateMipmaps() { }

	virtual void Resize(unsigned int width, unsigned int height);

private:
	bool CopyToSystemTexture();
	TemporaryTexture m_pTempTextureForCopy;
};

class NoLockableTexture2D : public Texture2D
{
public:
	NoLockableTexture2D(IDirect3DTexture9*, RenderAPI::TextureFormat, RenderAPI::ResourceUsage, bool isManaged,
		unsigned int width, unsigned int height, bool autoGenMipmaps, IInternalLogger&);

	~NoLockableTexture2D();
	
	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void GenerateMipmaps();

private:
	TemporaryTexture m_pTempTextureForUpdate;
};

//为了保证上层接口的方便易用性，我们专门提供了一个为RenderTarget使用的Texture
//这个Texture除了获取Surface0，其他的操作都会失败。
//上层不会饱受 if-else的困扰
class RenderSurface2D : public RenderAPI::Texture2D, public ::TextureSurface
{
public:
	RenderSurface2D(IDirect3DSurface9*, RenderAPI::TextureFormat, unsigned int width, unsigned int height, IInternalLogger&);

	~RenderSurface2D();

	//Texture
	virtual RenderAPI::TextureFormat GetFormat() const { return m_texFormat; }

	virtual void GenerateMipmaps() { }

	virtual bool AutoGenMipmaps() const { return false; }

	virtual bool IsCubemap() const { return false; }

	virtual bool NeedRecreateWhenDeviceLost() const { return false; }

	virtual void SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter) { }
	
	virtual unsigned int GetLength()const { return 0; }

	//Texture2D
	virtual unsigned int GetWidth() const { return m_texWidth; }

	virtual unsigned int GetHeight() const{ return m_texHeight; }

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual unsigned int GetLayerCount() const { return 1; }

	virtual RenderAPI::TextureSurface* GetSurface(unsigned int layer);

	virtual bool IsRenderTexture() const { return true; }

	virtual bool RenderAPI::Texture2D::SaveToFile(const char* fileName, RenderAPI::ImageFormat format)
	{
		return TextureSurface::SaveToFile(fileName, format);
	}

	virtual unsigned int RenderAPI::Texture2D::AddReference()
	{
		return TextureSurface::AddReference();
	}

	virtual void RenderAPI::Texture2D::Release()
	{
		TextureSurface::Release();
	}

private:
	bool CopyToSystemTexture();
	TemporaryTexture m_pTempTextureForCopy;
	IInternalLogger& m_internalLogger;
	const RenderAPI::TextureFormat m_texFormat;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
};