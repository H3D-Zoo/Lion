#include "VertexDeclaration.h"

VertexDeclaration::VertexDeclaration(const RenderAPI::VertexElement* elements, unsigned int elementCount)
	: m_vertexElements(elements, elements + elementCount)
{
}

const RenderAPI::VertexElement * VertexDeclaration::GetElements() const
{
	return &(m_vertexElements[0]);
}

unsigned int VertexDeclaration::GetElementCount() const
{
	return m_vertexElements.size();;
}

void VertexDeclaration::Release()
{
	delete this;
}
