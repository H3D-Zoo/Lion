#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "DX9Include.h"

class VertexDeclaration : public RenderAPI::VertexDeclaration
{
public:
	VertexDeclaration(IDirect3DVertexDeclaration9* pDeclaration, const std::vector<RenderAPI::VertexElement>& elements);

	~VertexDeclaration();

	virtual const RenderAPI::VertexElement* GetElements() const;

	virtual unsigned int GetElementCount() const;

	IDirect3DVertexDeclaration9* GetD3DVertexDeclarationPtr();

	virtual void Release();

private:
	std::vector<RenderAPI::VertexElement> m_vertexElements;
	IDirect3DVertexDeclaration9* m_pDeclaration;
};