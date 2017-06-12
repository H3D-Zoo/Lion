#include "DepthStencil.h"

DepthStencil::DepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
{

}

RenderAPI::ZBufferFormat DepthStencil::GetFormat() const
{
	return m_format;
}

unsigned int DepthStencil::GetWidth() const
{
	return m_width;
}

unsigned int DepthStencil::GetHeight() const
{
	return m_height;
}

void DepthStencil::Release()
{
	if (m_refCount.OnlyOne())
	{
		delete this;
	}
	else
	{
		--m_refCount;
	}
}

void DepthStencil::AddRef()
{
	++m_refCount;
}
