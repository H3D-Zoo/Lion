#pragma once

#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"
#include "RefCount.hpp"

class VertexDeclaration : public RenderAPI::VertexDeclaration
{
public:
	VertexDeclaration(IDirect3DVertexDeclaration9*, const std::vector<RenderAPI::VertexElement>&);

	~VertexDeclaration();

	virtual const RenderAPI::VertexElement* GetElements() const;

	virtual unsigned int GetElementCount() const;

	virtual unsigned int AddReference();

	virtual void Release();

	IDirect3DVertexDeclaration9* GetD3DVertexDeclarationPtr();

private:
	RefCount m_refCount;
	const std::vector<RenderAPI::VertexElement> m_vertexElements;
	IDirect3DVertexDeclaration9* m_pDeclaration;
};