#include "VertexBuffer.h"
#include "EnumMapping.h"

VertexBuffer::VertexBuffer(IDirect3DVertexBuffer9* vertexBuffer, RenderAPI::ResourceUsage usage, bool isManaged, unsigned int vertexCount, unsigned int vertexSize, IInternalLogger& logger)
	: m_internalLogger(logger)
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
	LOG_FUNCTION_V(m_internalLogger, "object=%X, offset=%d, lockLength=%d, lockOption=%d", 
		this, offset, lockLength, lockOption);

	if (m_writeOnly && lockOption == RenderAPI::LOCK_ReadOnly)
	{
		LOG_FUNCTION_W(m_internalLogger, "write-only buffers cannot be read");
		return NULL;
	}
	else if (lockOption == RenderAPI::LOCK_NoOverWrite || lockOption == RenderAPI::LOCK_Discard)
	{
		//The D3DLOCK_DISCARD and D3DLOCK_NOOVERWRITE flags are valid only 
		//on buffers created with D3DUSAGE_DYNAMIC.
		if (!m_isDynamic)
		{
			LOG_FUNCTION_V(m_internalLogger, "reset lock option to normal.");
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
		LOG_FUNCTION_W(m_internalLogger, "failed, error=%X", hr);
		return NULL;
	}
}

void* VertexBuffer::LockAll(RenderAPI::LockOption lockOption)
{
	return Lock(0, 0, lockOption);
}

void VertexBuffer::Unlock()
{
	LOG_FUNCTION_CALL(m_internalLogger, LOG_Verbose);
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
