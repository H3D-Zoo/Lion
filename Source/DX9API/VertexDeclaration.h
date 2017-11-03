#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "DX9Include.h"
#include "RefCount.hpp"

class VertexDeclaration : public RenderAPI::VertexDeclaration
{
public:
	VertexDeclaration(IDirect3DVertexDeclaration9* pDeclaration, const std::vector<RenderAPI::VertexElement>& elements);

	~VertexDeclaration();

	virtual const RenderAPI::VertexElement* GetElements() const;

	virtual unsigned int GetElementCount() const;

	IDirect3DVertexDeclaration9* GetD3DVertexDeclarationPtr();

	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
	std::vector<RenderAPI::VertexElement> m_vertexElements;
	IDirect3DVertexDeclaration9* m_pDeclaration;
};