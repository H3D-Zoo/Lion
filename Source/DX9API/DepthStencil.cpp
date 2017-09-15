#include "DepthStencil.h"

DepthStencil::DepthStencil(IDirect3DSurface9* dsSurface, RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_dsSurface(dsSurface)
{

}

DepthStencil::~DepthStencil()
{
	m_dsSurface->Release();
	m_dsSurface = NULL;
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

void DepthStencil::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void DepthStencil::AddRef()
{
	++m_refCount;
}

void DepthStencil::ReleaseWhenDeviceLost()
{
	m_dsSurface->Release();
	m_dsSurface = NULL;
}

void DepthStencil::Reset(unsigned int width, unsigned int height, RenderAPI::DepthStencilFormat dsFormat, IDirect3DSurface9 * pSurface)
{
	m_width = width;
	m_height = height;
	m_format = dsFormat;
	// 这时候应该是被释放状态	
	m_dsSurface = pSurface;
}

IDirect3DSurface9 * DepthStencil::GetD3DSurface() const
{
	return m_dsSurface;
}
