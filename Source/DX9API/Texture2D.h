#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"

class RenderTarget;
class DepthStencil;

class Texture2D : public RenderAPI::Texture2D
{
public:
	Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height);

	Texture2D(IDirect3DTexture9* texture, RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, void* initialData);

	~Texture2D();

	virtual RenderAPI::TextureFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption);

	virtual void UnlockRect(unsigned int layer);

	virtual void Release();

	void Resize(unsigned int width, unsigned int height);

	IDirect3DTexture9* GetD3DTexture();

private:
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::TextureFormat m_texFormat;
	IDirect3DTexture9* m_pTexture;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
};
