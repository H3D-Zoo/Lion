#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"

class Texture2D;

class RenderTarget : public RenderAPI::RenderTarget
{
public:
	RenderTarget(RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height, bool isTexture);

	~RenderTarget();

	virtual RenderAPI::RenderTargetFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool IsTexture2D() const;

	virtual RenderAPI::Texture2D* GetTexturePtr();

	virtual void Release();

	void AddRef(); 
	
	void Resize(unsigned int width, unsigned int height);



private:
	RefCount m_refCount;
	RenderAPI::RenderTargetFormat m_format;
	::Texture2D* m_texture;
	unsigned int m_width;
	unsigned int m_height;
};