#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize)
	: m_usage(usage)
	, m_vertexCount(vertexCount)
	, m_bufferLength(vertexCount * vertexSize)
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