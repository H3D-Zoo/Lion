#include "IndexBuffer.h"

namespace
{
	unsigned int s_IndexLength[] =
	{
		sizeof(unsigned short),
		sizeof(unsigned int),
	};
}

IndexBuffer::IndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int count)
	: m_usage(usage)
	, m_indexFormat(format)
	, m_indexCount(count)
{
	m_bufferLength = s_IndexLength[format] * m_indexCount;
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

void * IndexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	return NULL;
}

void * IndexBuffer::DiscardLock()
{
	return NULL;
}

inline void IndexBuffer::Unlock()
{

}

bool IndexBuffer::NeedRecreateWhenDeviceLost() 
{
	return false;
}

void IndexBuffer::Release()
{
	delete this;
}
