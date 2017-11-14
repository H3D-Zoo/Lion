#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "APIInstance.h"
#include "RefCount.hpp"


class RenderTarget;
class DepthStencil;
class Texture2D;

class TextureSurface : public RenderAPI::TextureSurface
{
public:
	TextureSurface(Texture2D* pTexture);

	virtual void* GetDC();

	virtual void ReleaseDC();

	virtual bool SaveToFile(const char* fileName, RenderAPI::ImageFormat format);

	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
	Texture2D* m_pParentTexture;
};

class Texture2D : public RenderAPI::Texture2D
{
public:
	Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps);

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

	unsigned int ClearStamp;

protected:
	unsigned int LinePitch() const;

	const bool m_autoGenMipmaps;
	const bool m_isManaged;
	const bool m_isDynamic;
	const RenderAPI::ResourceUsage m_usage;
	const RenderAPI::TextureFormat m_texFormat;
	const unsigned int m_layerCount;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
private:
	RefCount m_refCount;
	std::vector<TextureSurface*> m_surfaces;
	std::vector<char> m_buffer;
};

class RenderTexture2D : public Texture2D
{
public:
	RenderTexture2D(APIInstance* pAPIInstance, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage,
		unsigned int width, unsigned int height);

	~RenderTexture2D();

	virtual bool IsRenderTexture() const { return true; }

	virtual bool NeedRecreateWhenDeviceLost() const { return false; }

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void SetMipmapGenerateFilter(RenderAPI::SamplerFilter filter) { }

	virtual void GenerateMipmaps() { }

	virtual void Resize(unsigned int width, unsigned int height);
};