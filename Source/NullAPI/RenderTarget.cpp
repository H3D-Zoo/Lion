#include "RenderTarget.h"

RenderTarget::RenderTarget(RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
{

}

RenderAPI::BackBufferFormat RenderTarget::GetFormat() const
{
	return m_format;
}

unsigned int RenderTarget::GetWidth() const
{
	return m_width;
}

unsigned int RenderTarget::GetHeight() const
{
	return m_height;
}

void RenderTarget::Release()
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

void RenderTarget::AddRef()
{
	++m_refCount;
}

void RenderTarget::Resize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
}
