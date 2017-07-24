#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"
#include "Texture2D.h"
#include "DX9Include.h"

class DepthStencil : public RenderAPI::DepthStencil
{
public:
	DepthStencil(IDirect3DSurface9* dsSurface, RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);

	DepthStencil(IDirect3DTexture9* dsSurface, RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);

	~DepthStencil();

	virtual RenderAPI::DepthStencilFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool IsTexture2D() const;

	virtual RenderAPI::Texture2D* GetTexturePtr();

	virtual void Release();

	void AddRef();

	void ReleaseWhenDeviceLost();

	void Reset(unsigned int width, unsigned int height, RenderAPI::DepthStencilFormat dsFormat, IDirect3DSurface9* pSurface);

	IDirect3DSurface9* GetD3DSurface() const;

private:
	RefCount m_refCount;
	RenderAPI::DepthStencilFormat m_format;
	unsigned int m_width;
	unsigned int m_height;
	IDirect3DSurface9* m_dsSurface;
	::Texture2D* m_dsTexture;
};