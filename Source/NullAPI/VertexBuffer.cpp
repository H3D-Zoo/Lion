#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, IInternalLogger& logger)
	: m_internalLogger(logger)
	, m_usage(usage)
	, m_isManaged(usage == RenderAPI::RESUSAGE_DynamicManaged || usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_StaticWOManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_writeOnly(!(usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_Static))
	, m_vertexCount(vertexCount)
	, m_vertexStride(vertexSize)
	, m_bufferLength(vertexCount * vertexSize)
{
	m_buffer.resize(m_bufferLength);
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

void * VertexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{

	if (m_writeOnly && lockOption == RenderAPI::LOCK_ReadOnly)
	{
		m_internalLogger.LogError("VertexBuffer::Lock", "Can not lock write-only buffer.");
		return NULL;
	}
	else
	{
		return &(m_buffer[0]);
	}
}

void* VertexBuffer::LockAll(RenderAPI::LockOption lockOption)
{
	return Lock(0, 0, lockOption);
}

void VertexBuffer::Unlock()
{

}

bool VertexBuffer::NeedRecreateWhenDeviceLost() const
{
	return !m_isManaged;
}

unsigned int VertexBuffer::AddReference()
{
	return ++m_refCount;
}

void VertexBuffer::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}