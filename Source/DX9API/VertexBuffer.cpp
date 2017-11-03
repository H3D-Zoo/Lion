#include "VertexBuffer.h"
#include "EnumMapping.h"

VertexBuffer::VertexBuffer(APIInstance* pAPIInstance, IDirect3DVertexBuffer9* vertexBuffer, RenderAPI::ResourceUsage usage, bool isManaged, unsigned int vertexCount, unsigned int vertexSize)
	: m_pAPIInstance(pAPIInstance)
	, m_usage(usage)
	, m_isManaged(isManaged)
	, m_isDynamic(usage == RenderAPI::RESUSAGE_Dynamic || usage == RenderAPI::RESUSAGE_DynamicManaged)
	, m_writeOnly(!(usage == RenderAPI::RESUSAGE_StaticManaged || usage == RenderAPI::RESUSAGE_Static))
	, m_vertexCount(vertexCount)
	, m_vertexStride(vertexSize)
	, m_bufferLength(vertexCount * vertexSize)
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

void * VertexBuffer::Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption)
{
	if (m_writeOnly && lockOption == RenderAPI::LOCK_ReadOnly)
	{
		return NULL;
	}
	else if (lockOption == RenderAPI::LOCK_NoOverWrite || lockOption == RenderAPI::LOCK_Discard)
	{
		//The D3DLOCK_DISCARD and D3DLOCK_NOOVERWRITE flags are valid only 
		//on buffers created with D3DUSAGE_DYNAMIC.
		if (!m_isDynamic)
		{
			lockOption = RenderAPI::LOCK_Normal;
		}
	}

	void* pDataPtr = NULL;
	HRESULT hr = m_pVertexBuffer->Lock(offset, lockLength, &pDataPtr, s_lockOptions[lockOption]);
	if (S_OK == hr)
	{
		return pDataPtr;
	}
	else
	{
		m_pAPIInstance->LogError("VertexBuffer::Lock", "Lock failed.", hr);
		return NULL;
	}
}

void* VertexBuffer::LockAll(RenderAPI::LockOption lockOption)
{
	return Lock(0, 0, lockOption);
}

void VertexBuffer::Unlock()
{
	m_pVertexBuffer->Unlock();
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

IDirect3DVertexBuffer9 * VertexBuffer::GetBufferPtr()
{
	return m_pVertexBuffer;
}
