#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "APIInstance.h"


class IndexBuffer : public RenderAPI::IndexBuffer
{
public:
	IndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int count);
	
	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual RenderAPI::IndexFormat GetFormat() const;

	virtual unsigned int GetIndexCount() const;

	virtual unsigned int GetLength() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* LockAll(RenderAPI::LockOption lockOption);

	virtual void Unlock();

	virtual unsigned int AddReference();

	virtual void Release();

	virtual bool NeedRecreateWhenDeviceLost() const;

	bool IsDynamic() { return m_isDynamic; }

private:
	RefCount m_refCount;
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::IndexFormat m_indexFormat;
	const bool m_isManaged;
	const bool m_isDynamic;
	const bool m_writeOnly;
	const unsigned int m_indexCount;
	const unsigned int m_bufferLength;

	std::vector<char> m_buffer;
};