#pragma once
#include "../../RenderAPI/RenderAPI.h"

class RenderTarget;
class DepthStencil;

class Texture2D : public RenderAPI::Texture2D
{
public:
	Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, bool rt, bool ds);

	Texture2D(RenderAPI::TextureFormat format, RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, void* initialData, bool rt, bool ds);

	virtual RenderAPI::TextureFormat GetFormat() const; 
	
	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool IsRenderTarget() const;

	virtual RenderAPI::RenderTarget* GetRenderTarget() const;

	virtual bool IsDepthStencil() const;

	virtual RenderAPI::DepthStencil* GetDepthStencil() const;

	virtual void Release();

	void Resize(unsigned int width, unsigned int height);

private:
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::TextureFormat m_texFormat;
	unsigned int m_texWidth;
	unsigned int m_texHeight;
	bool m_isRenderTarget;
	bool m_isDepthStencil;
	mutable ::RenderTarget* m_renderTarget;
	mutable ::DepthStencil* m_depthStencil;
};
