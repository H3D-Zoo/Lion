#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::Semantic * semantics, unsigned int semanticCount, void * initialData)
	: m_usage(usage)
	, m_vertexCount(vertexCount)
	, m_bufferLength(vertexCount * vertexSize)
	, m_semantics(semantics, semantics + semanticCount)
{
}

VertexBuffer::VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::Semantic * semantics, unsigned int semanticCount)
	: m_usage(usage)
	, m_vertexCount(vertexCount)
	, m_bufferLength(vertexCount * vertexSize)
	, m_semantics(semantics, semantics + semanticCount)
{
}

RenderAPI::ResourceUsage VertexBuffer::GetUsage() const
{
	return m_usage;
}

unsigned int VertexBuffer::GetVertexCount() const
{
	return m_vertexCount;
}

unsigned int VertexBuffer::GetLength() const
{
	return m_bufferLength;
}

const RenderAPI::Semantic * VertexBuffer::GetSemanticPtr() const
{
	return &(m_semantics[0]);
}

unsigned int VertexBuffer::GetSemanticCount() const
{
	return m_semantics.size();
}

void VertexBuffer::Release()
{
	delete this;
}