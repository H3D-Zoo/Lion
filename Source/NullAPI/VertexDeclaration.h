#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>

class VertexDeclaration : public RenderAPI::VertexDeclaration
{
public:
	VertexDeclaration(const RenderAPI::VertexElement* elements, unsigned int elementCount);
	
	virtual const RenderAPI::VertexElement* GetElements() const;

	virtual unsigned int GetElementCount() const;

	virtual void Release();

private:
	std::vector<RenderAPI::VertexElement> m_vertexElements;
};