#include "VertexDeclaration.h"

VertexDeclaration::VertexDeclaration(const std::vector<RenderAPI::VertexElement>& elements)
	: m_vertexElements(elements)
{

}

VertexDeclaration::~VertexDeclaration()
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
