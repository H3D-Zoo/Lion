#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"

class Texture2D;

class DepthStencil : public RenderAPI::DepthStencil
{
public:
	DepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height, bool isTexture);

	~DepthStencil();

	virtual RenderAPI::DepthStencilFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool IsTexture2D() const;

	virtual RenderAPI::Texture2D* GetTexturePtr();

	virtual void Release();

	void AddRef();

private:
	RefCount m_refCount;
	RenderAPI::DepthStencilFormat m_format;
	::Texture2D* m_texture;
	unsigned int m_width;
	unsigned int m_height;
};