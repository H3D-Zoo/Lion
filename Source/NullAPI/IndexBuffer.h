#pragma once
#include "../../RenderAPI/RenderAPI.h"

#include <vector>

class IndexBuffer : public RenderAPI::IndexBuffer
{
public:
	IndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int count);

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual RenderAPI::IndexFormat GetFormat() const;

	virtual unsigned int GetIndexCount() const;

	virtual unsigned int GetLength() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* DiscardLock();

	virtual void Unlock();

	virtual void Release();
private:
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::IndexFormat m_indexFormat;
	unsigned int m_indexCount;
	unsigned int m_bufferLength;
};