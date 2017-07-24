#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"
#include "Texture2D.h"
#include "DX9Include.h"


class RenderTarget : public RenderAPI::RenderTarget
{
public:
	RenderTarget(IDirect3DSurface9* rtSurface, RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height);

	RenderTarget(IDirect3DTexture9* rtTexture, RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height);

	~RenderTarget();

	virtual RenderAPI::RenderTargetFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool IsTexture2D() const;

	virtual RenderAPI::Texture2D* GetTexturePtr();

	virtual void Release();

	void AddRef();

	void Resize(unsigned int width, unsigned int height);

	void ReleaseWhenDeviceLost();

	void Reset(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, IDirect3DSurface9* pSurface);

	IDirect3DSurface9* GetD3DSurface() const;

private:
	RefCount m_refCount;
	RenderAPI::RenderTargetFormat m_format;
	unsigned int m_width;
	unsigned int m_height;
	IDirect3DSurface9* m_rtSurface;
	::Texture2D* m_rtTexture;
};