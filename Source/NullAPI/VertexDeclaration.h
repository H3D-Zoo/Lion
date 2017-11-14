#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>

#include "RefCount.hpp"

class VertexDeclaration : public RenderAPI::VertexDeclaration
{
public:
	VertexDeclaration(const std::vector<RenderAPI::VertexElement>& elements);

	~VertexDeclaration();

	virtual const RenderAPI::VertexElement* GetElements() const;

	virtual unsigned int GetElementCount() const;
	
	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
	const std::vector<RenderAPI::VertexElement> m_vertexElements;
};