#pragma once
#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "InternalLogger.h"
#include "RefCount.hpp"


class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, IInternalLogger& logger);

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

	bool IsDynamic() {return m_isDynamic;}

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
	std::vector<char> m_buffer;
};