#include "DepthStencil.h"

namespace
{
	RenderAPI::TextureFormat s_dsTexFormats[] =
	{
		RenderAPI::TEX_D24S8,
		RenderAPI::TEX_D24X8,
		RenderAPI::TEX_D16,
	};
}

DepthStencil::DepthStencil(IDirect3DSurface9* dsSurface, RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_dsSurface(dsSurface)
	, m_dsTexture(NULL)
{

}

DepthStencil::DepthStencil(IDirect3DTexture9* dsTexture, RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height)
	: m_format(format)
	, m_width(width)
	, m_height(height)
	, m_dsSurface(NULL)
	, m_dsTexture(NULL)
{

	dsTexture->GetSurfaceLevel(0, &m_dsSurface);
	m_dsSurface = m_dsSurface;
	m_dsTexture = new Texture2D(dsTexture, s_dsTexFormats[format], RenderAPI::RESUSAGE_Default, width, height);
}


DepthStencil::~DepthStencil()
{
	m_dsSurface->Release();
	m_dsSurface = NULL;
	if (m_dsTexture != NULL)
	{
		m_dsTexture->Release();
		m_dsTexture = NULL;
	}
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

bool DepthStencil::IsTexture2D() const
{
	return m_dsTexture != NULL;
}

RenderAPI::Texture2D* DepthStencil::GetTexturePtr()
{
	return m_dsTexture;
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
	// ��ʱ��Ӧ���Ǳ��ͷ�״̬	
	m_dsSurface = pSurface;
}

IDirect3DSurface9 * DepthStencil::GetD3DSurface() const
{
	return m_dsSurface;
}
