#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "APIInstance.h"
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

	virtual bool SaveToFile(const char* fileName, RenderAPI::ImageFormat format);

	virtual unsigned int AddReference();

	virtual void Release();

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
	Texture2D(APIInstance* pAPIInstance, IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged,
		unsigned int width, unsigned int height, bool autoGenMipmaps);

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
	APIInstance* m_pAPIInstance;

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

class RenderTexture2D : public Texture2D
{
public:
	RenderTexture2D(APIInstance* pAPIInstance, IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage,
		unsigned int width, unsigned int height);

	~RenderTexture2D();

	virtual bool IsRenderTexture() const { return true; }

	virtual bool NeedRecreateWhenDeviceLost() const { return false; }

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter) { }

	virtual void GenerateMipmaps() { }

	virtual void Resize(unsigned int width, unsigned int height);

private:
	IDirect3DTexture9* GetCopiedSystemTexture();

	void ReleaseCopiedSystemTexture();

	IDirect3DTexture9* m_pTempTextureForCopy;
};

class NoLockableTexture2D : public Texture2D
{
public:
	NoLockableTexture2D(APIInstance* pAPIInstance, IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, bool isManaged,
		unsigned int width, unsigned int height,
		bool autoGenMipmaps);

	~NoLockableTexture2D();
	
	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void GenerateMipmaps();

private:
	void SetLayerLocking(unsigned int layer, bool locked);

	bool IsLayerLocking(unsigned int layer) const;

	bool IsSomeLayerLocking() const;

	IDirect3DTexture9* m_pTempTextureForUpdate;
	
	unsigned int m_lockLayerBits;
};

//Ϊ�˱�֤�ϲ�ӿڵķ��������ԣ�����ר���ṩ��һ��ΪRenderTargetʹ�õ�Texture
//���Texture���˻�ȡSurface0�������Ĳ�������ʧ�ܡ�
//�ϲ㲻�ᱥ�� if-else������
class RenderSurface2D : public RenderAPI::Texture2D, public ::TextureSurface
{
public:
	RenderSurface2D(APIInstance* pAPIInstance, IDirect3DSurface9* pSurface, RenderAPI::TextureFormat format, unsigned int width, unsigned int height);

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
	IDirect3DTexture9* GetCopiedSystemTexture();

	void ReleaseCopiedSystemTexture();

	APIInstance* m_pAPIInstance;
	IDirect3DTexture9* m_pTempTextureForCopy;

	const RenderAPI::TextureFormat m_texFormat;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
};