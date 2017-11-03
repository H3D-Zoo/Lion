#include "VertexDeclaration.h"

VertexDeclaration::VertexDeclaration(IDirect3DVertexDeclaration9* pDeclaration, const std::vector<RenderAPI::VertexElement>& elements)
	: m_vertexElements(elements)
	, m_pDeclaration(pDeclaration)
{
}

VertexDeclaration::~VertexDeclaration()
{
	m_pDeclaration->Release();
}

const RenderAPI::VertexElement * VertexDeclaration::GetElements() const
{
	return &(m_vertexElements[0]);
}

unsigned int VertexDeclaration::GetElementCount() const
{
	return m_vertexElements.size();;
}

IDirect3DVertexDeclaration9 * VertexDeclaration::GetD3DVertexDeclarationPtr()
{
	return m_pDeclaration;
}

unsigned int VertexDeclaration::AddReference()
{
	return ++m_refCount;
}

void VertexDeclaration::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}
