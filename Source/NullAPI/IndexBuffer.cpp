#include "IndexBuffer.h"

const unsigned int kIndexLengthCount = 2;
unsigned int s_IndexLengths[kIndexLengthCount] =
{
	sizeof(unsigned short),
	sizeof(unsigned int),
};

IndexBuffer::IndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int count)
	: m_usage(usage)
	, m_indexFormat(format)
	, m_isManaged(usage == RenderAPI::RESUSAGE_DynamicManaged || usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_StaticWOManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_writeOnly(!(usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_Static))
	, m_indexCount(count)
	, m_bufferLength(s_IndexLengths[format] * count)
{

	m_buffer.resize(m_bufferLength);
}

RenderAPI::ResourceUsage IndexBuffer::GetUsage() const
{
	return m_usage;
}

RenderAPI::IndexFormat IndexBuffer::GetFormat() const
{
	return m_indexFormat;
}

unsigned int IndexBuffer::GetIndexCount() const
{
	return m_indexCount;
}

unsigned int IndexBuffer::GetLength() const
{
	return m_bufferLength;
}

bool IndexBuffer::NeedRecreateWhenDeviceLost() const
{
	return !m_isManaged;
}

void * IndexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	if (m_writeOnly && lockOption == RenderAPI::LOCK_ReadOnly)
	{
		return NULL;
	}
	else
	{
		if (lockOption == RenderAPI::LOCK_NoOverWrite || lockOption == RenderAPI::LOCK_Discard)
		{
			//The D3DLOCK_DISCARD and D3DLOCK_NOOVERWRITE flags are valid only 
			//on buffers created with D3DUSAGE_DYNAMIC.
			if (!m_isDynamic)
			{
				lockOption = RenderAPI::LOCK_Normal;
			}
		}

		return &(m_buffer[0]);
	}
}

void* IndexBuffer::LockAll(RenderAPI::LockOption lockOption)
{
	return Lock(0, 0, lockOption);
}

void IndexBuffer::Unlock()
{

}

unsigned int IndexBuffer::AddReference()
{
	return ++m_refCount;
}

void IndexBuffer::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}
