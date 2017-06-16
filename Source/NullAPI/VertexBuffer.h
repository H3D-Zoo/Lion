#pragma once
#include "../../RenderAPI/RenderAPI.h"

#include <vector>

class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::Semantic * semantics, unsigned int semanticCount, void* initialData);

	VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::Semantic * semantics, unsigned int semanticCount);

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual unsigned int GetVertexCount() const;

	virtual unsigned int GetVertexStride() const;

	virtual unsigned int GetLength() const;

	virtual const RenderAPI::Semantic* GetSemanticPtr() const;

	virtual unsigned int GetSemanticCount() const;

	virtual void Release();

private:
	RenderAPI::ResourceUsage m_usage;
	unsigned int m_vertexCount;
	unsigned int m_vertexStride;
	unsigned int m_bufferLength;
	const std::vector<RenderAPI::Semantic> m_semantics;
};