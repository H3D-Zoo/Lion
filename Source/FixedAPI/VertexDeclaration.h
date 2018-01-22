#pragma once

#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"
#include "RefCount.hpp"

class VertexDeclaration : public RenderAPI::VertexDeclaration
{
public:
	VertexDeclaration(const std::vector<RenderAPI::VertexElement>&, unsigned int fvf);

	virtual const RenderAPI::VertexElement* GetElements() const;

	virtual unsigned int GetElementCount() const;

	virtual unsigned int AddReference();

	virtual void Release();

	unsigned int GetCustomFVF() const;

private:
	RefCount m_refCount;
	const std::vector<RenderAPI::VertexElement> m_vertexElements;
	const unsigned int m_customFVF;
};