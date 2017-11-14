#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"
#include "Texture2D.h"


class DepthStencil : public RenderAPI::DepthStencil
{
public:
	DepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::DepthStencilFormat GetFormat() const;

	virtual unsigned int GetWidth() const;

	virtual unsigned int GetHeight() const;

	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
	const RenderAPI::DepthStencilFormat m_format;
	const unsigned int m_width;
	const unsigned int m_height;
};