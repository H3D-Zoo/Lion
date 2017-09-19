#include "IndexBuffer.h"
#include "EnumMapping.h"

IndexBuffer::IndexBuffer(APIInstance* pAPI, IDirect3DIndexBuffer9* indexBuffer, RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int count, bool recreateWhenDeviceLost)
	: m_pAPIInstance(pAPI)
	, m_usage(usage)
	, m_indexFormat(format)
	, m_recreateWhenDeviceLost(recreateWhenDeviceLost)
	, m_indexCount(count)
	, m_pIndexBuffer(indexBuffer)
{
	m_bufferLength = s_IndexLengths[format] * m_indexCount;
}

IndexBuffer::~IndexBuffer()
{
	m_pIndexBuffer->Release();
	m_pIndexBuffer = NULL;
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

void IndexBuffer::Release()
{
	delete this;
}

bool IndexBuffer::NeedRecreateWhenDeviceLost() const
{
	return m_recreateWhenDeviceLost;
}

IDirect3DIndexBuffer9 * IndexBuffer::GetD3DIndexBuffer()
{
	return m_pIndexBuffer;
}
void * IndexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	if (m_usage == RenderAPI::RESUSAGE_Immuable)
	{
		m_pAPIInstance->LogError("IndexBuffer::Lock", "Immuable IndexBuffer Cannot be locked.");
		return NULL;
	}

	void* pDataPtr = NULL;
	HRESULT hr = m_pIndexBuffer->Lock(offset, lockLength, &pDataPtr, GetLockOption(lockOption,m_usage));
	if (S_OK == hr)
	{
		return pDataPtr;
	}
	else
	{
		m_pAPIInstance->LogError("IndexBuffer::Lock", "IndexBuffer Cannot be locked.", hr);
		return NULL;
	}
}

void* IndexBuffer::DiscardLock()
{
	return Lock(0, 0, RenderAPI::LOCK_Discard);
}

void IndexBuffer::Unlock()
{
	m_pIndexBuffer->Unlock();
}