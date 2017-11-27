#include "VertexDeclaration.h"

VertexDeclaration::VertexDeclaration(const std::vector<RenderAPI::VertexElement>& elements, unsigned int fvf)
	: m_vertexElements(elements)
	, m_customFVF(fvf)
{
}

const RenderAPI::VertexElement * VertexDeclaration::GetElements() const
{
	return &(m_vertexElements[0]);
}

unsigned int VertexDeclaration::GetElementCount() const
{
	return static_cast<unsigned int>(m_vertexElements.size());
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

unsigned int VertexDeclaration::GetCustomFVF() const
{
	return m_customFVF;
}
