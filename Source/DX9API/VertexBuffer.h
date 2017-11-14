#pragma once
#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"
#include "InternalLogger.h"

class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(IDirect3DVertexBuffer9*, RenderAPI::ResourceUsage, bool isManaged, unsigned int vertexCount, unsigned int vertexSize, IInternalLogger&);

	~VertexBuffer();

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual unsigned int GetVertexCount() const;

	virtual unsigned int GetVertexStride() const;

	virtual unsigned int GetLength() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* LockAll(RenderAPI::LockOption lockOption);

	virtual void Unlock();
	
	virtual bool NeedRecreateWhenDeviceLost() const;

	virtual unsigned int AddReference();

	virtual void Release();

	IDirect3DVertexBuffer9* GetBufferPtr();

	bool IsDynamic() { return m_isDynamic; }

private:
	RefCount m_refCount;
	IInternalLogger& m_internalLogger;
	const RenderAPI::ResourceUsage m_usage;
	const bool m_isManaged;
	const bool m_isDynamic;
	const bool m_writeOnly;
	const unsigned int m_vertexCount;
	const unsigned int m_vertexStride;
	const unsigned int m_bufferLength;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
};