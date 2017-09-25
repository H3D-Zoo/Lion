#include "IndexBuffer.h"
#include "EnumMapping.h"

IndexBuffer::IndexBuffer(APIInstance* pAPI, IDirect3DIndexBuffer9* indexBuffer, RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, bool isManaged, unsigned int count)
	: m_pAPIInstance(pAPI)
	, m_usage(usage)
	, m_indexFormat(format)
	, m_isManaged(isManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_writeOnly(!(usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_StaticRW))
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
	return !m_isManaged;
}

IDirect3DIndexBuffer9 * IndexBuffer::GetD3DIndexBuffer()
{
	return m_pIndexBuffer;
}
void * IndexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	if (m_writeOnly && lockOption == RenderAPI::LOCK_ReadOnly)
	{
		return NULL;
	}
	else if (m_isDynamic && lockOption == RenderAPI::LOCK_Normal)
	{
		//The D3DLOCK_DISCARD and D3DLOCK_NOOVERWRITE flags are valid only 
		//on buffers created with D3DUSAGE_DYNAMIC.
		lockOption = RenderAPI::LOCK_Discard;
	}

	void* pDataPtr = NULL;
	HRESULT hr = m_pIndexBuffer->Lock(offset, lockLength, &pDataPtr, s_lockOptions[lockOption]);
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