#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"
#include "RefCount.hpp"
#include "Texture2D.h"

class DepthStencil : public RenderAPI::DepthStencil
{
public:
	DepthStencil(IDirect3DSurface9*, RenderAPI::DepthStencilFormat, unsigned int width, unsigned int height);

	~DepthStencil();

	virtual RenderAPI::DepthStencilFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual unsigned int AddReference();

	virtual void Release();

	void ReleaseWhenDeviceLost();

	void Reset(unsigned int width, unsigned int height, RenderAPI::DepthStencilFormat dsFormat, IDirect3DSurface9*);

	IDirect3DSurface9* GetD3DSurface() const;

private:
	RefCount m_refCount;
	RenderAPI::DepthStencilFormat m_format;
	unsigned int m_width;
	unsigned int m_height;
	IDirect3DSurface9* m_dsSurface;
};