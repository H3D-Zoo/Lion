#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "RenderAPIImpl.h"
#include "RenderTarget.h"
#include "DX9Include.h"


class SwapChain : public RenderAPI::SwapChain
{
public:
	SwapChain(IDirect3DDevice9* device, const RenderAPI::SwapChainDesc & swapChainDesc, bool fullscreen);

	~SwapChain();

	virtual RenderAPI::RenderTarget* GetRenderTarget() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual bool OnResize(unsigned int width, unsigned int height);

	virtual void SetFullscreen(bool fullscreen);

	virtual bool IsFullscreen() const;

	virtual void Present();

	virtual void Release();

	void AddRef();

private:
	RefCount m_refCount;
	bool m_isFullscreen;
	mutable ::RenderTarget m_renderTarget;
	IDirect3DDevice9* m_pDevice;
};