#pragma once
#include "../../RenderAPI/RenderAPI.h"

#include <vector>

class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::VertexElement* elements, unsigned int elementCount);

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual unsigned int GetVertexCount() const;

	virtual unsigned int GetVertexStride() const;

	virtual unsigned int GetLength() const;

	virtual const RenderAPI::VertexElement* GetElementPtr() const;

	virtual unsigned int GetElementCount() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* DiscardLock();

	virtual void Unlock();

	virtual void Release();

private:
	RenderAPI::ResourceUsage m_usage;
	unsigned int m_vertexCount;
	unsigned int m_vertexStride;
	unsigned int m_bufferLength;
	const std::vector<RenderAPI::VertexElement> m_vertexElements;
};