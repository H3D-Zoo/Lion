#include "RHIImpl.h"

TextureSampler::TextureSampler(H3D::SamplerFilter filter, H3D::TextureAddress addressU, H3D::TextureAddress addressV)
	: m_filer(filter)
	, m_addressU(addressU)
	, m_addressV(addressV)
{

}

H3D::SamplerFilter TextureSampler::GetFilter() const
{
	return m_filer;
}

H3D::TextureAddress TextureSampler::GetAddressU() const
{
	return m_addressU;
}

H3D::TextureAddress TextureSampler::GetAddressV() const
{
	return m_addressV;
}

void TextureSampler::Release()
{
	delete this;
}
