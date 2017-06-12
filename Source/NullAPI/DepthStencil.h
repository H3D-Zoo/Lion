#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "RenderAPIImpl.h"

class DepthStencil : public RenderAPI::DepthStencil
{
public:
	DepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::ZBufferFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual void Release();

	void AddRef();

private:
	RefCount m_refCount;
	RenderAPI::ZBufferFormat m_format;
	unsigned int m_width;
	unsigned int m_height;
};