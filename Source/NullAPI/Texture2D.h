#pragma once
#include "../../RenderAPI/RenderAPI.h"

class RenderTarget;
class DepthStencil;

class Texture2D : public RenderAPI::Texture2D
{
public:
	Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height);

	virtual RenderAPI::TextureFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual RenderAPI::MappedResource LockRect(unsigned int layer, RenderAPI::LockOption lockOption) ;

	virtual void UnlockRect(unsigned int layer);

	virtual void GenerateMipmaps();

	virtual void Release();

	void Resize(unsigned int width, unsigned int height);

private:
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::TextureFormat m_texFormat;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
};
