#pragma once
#include "../../RenderAPI/RenderAPI.h"

#include <vector>

class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize);

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual unsigned int GetVertexCount() const;

	virtual unsigned int GetVertexStride() const;

	virtual unsigned int GetLength() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* DiscardLock();

	virtual void Unlock();

	virtual bool NeedRecreateWhenDeviceLost();

	virtual void Release();

private:
	RenderAPI::ResourceUsage m_usage;
	unsigned int m_vertexCount;
	unsigned int m_vertexStride;
	unsigned int m_bufferLength;
};