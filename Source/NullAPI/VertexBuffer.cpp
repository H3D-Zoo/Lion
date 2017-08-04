#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::VertexElement* elements, unsigned int elementCount)
	: m_usage(usage)
	, m_vertexCount(vertexCount)
	, m_bufferLength(vertexCount * vertexSize)
	, m_vertexElements(elements, elements + elementCount)
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

unsigned int VertexBuffer::GetVertexStride() const
{
	return m_vertexStride;
}

unsigned int VertexBuffer::GetLength() const
{
	return m_bufferLength;
}

const RenderAPI::VertexElement * VertexBuffer::GetElementPtr() const
{
	return &(m_vertexElements[0]);
}

unsigned int VertexBuffer::GetElementCount() const
{
	return m_vertexElements.size();
}

void* VertexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	return NULL;
}

void* VertexBuffer::DiscardLock()
{
	return NULL;
}

void VertexBuffer::Unlock()
{

}

bool VertexBuffer::NeedRecreateWhenDeviceLost() 
{
	return false;
}

void VertexBuffer::Release()
{
	delete this;
}