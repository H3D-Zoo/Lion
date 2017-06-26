#include "VertexBuffer.h"
#include "RenderAPIImpl.h"

VertexBuffer::VertexBuffer(IDirect3DVertexBuffer9* vertexBuffer, RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::VertexElement * elements, unsigned int elementCount)
	: m_usage(usage)
	, m_vertexCount(vertexCount)
	, m_vertexStride(vertexSize)
	, m_bufferLength(vertexCount * vertexSize)
	, m_vertexElements(elements, elements + elementCount)
	, m_pVertexBuffer(vertexBuffer)
{
}

VertexBuffer::~VertexBuffer()
{
	m_pVertexBuffer->Release();
	m_pVertexBuffer = NULL;
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

void * VertexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	if (m_usage == RenderAPI::RESUSAGE_Immuable)
		return NULL;

	void* pDataPtr = NULL;
	if (S_OK == m_pVertexBuffer->Lock(offset, lockLength, &pDataPtr, s_lockOptions[lockOption]))
	{
		return pDataPtr;
	}
	else
	{
		return NULL;
	}
}

void* VertexBuffer::DiscardLock()
{
	return Lock(0, 0, RenderAPI::LOCK_Discard);
}

void VertexBuffer::Unlock()
{
	m_pVertexBuffer->Unlock();
}

void VertexBuffer::Release()
{
	delete this;
}

IDirect3DVertexBuffer9 * VertexBuffer::GetBufferPtr()
{
	return m_pVertexBuffer;
}
