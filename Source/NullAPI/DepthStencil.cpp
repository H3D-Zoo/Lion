#include "DepthStencil.h"

DepthStencil::DepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
{

}

RenderAPI::DepthStencilFormat DepthStencil::GetFormat() const
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

unsigned DepthStencil::AddReference()
{
	return ++m_refCount;
}

void DepthStencil::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}