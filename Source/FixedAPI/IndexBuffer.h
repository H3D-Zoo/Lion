#pragma once
#include <vector>
#include "../../RenderAPI/RenderAPI.h"
#include "APIInstance.h"
#include "DX9Include.h"

class IndexBuffer : public RenderAPI::IndexBuffer
{
public:
	IndexBuffer(IDirect3DIndexBuffer9*, RenderAPI::ResourceUsage, RenderAPI::IndexFormat, bool isManaged, unsigned int count, RenderAPI::Logger&);

	~IndexBuffer();

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

	IDirect3DIndexBuffer9* GetD3DIndexBuffer();

	bool IsDynamic() { return m_isDynamic; }

private:
	RefCount m_refCount;
	RenderAPI::Logger& m_internalLogger;
	const RenderAPI::ResourceUsage m_usage;
	const RenderAPI::IndexFormat m_indexFormat;
	const bool m_isManaged;
	const bool m_isDynamic;
	const bool m_writeOnly;
	const unsigned int m_indexCount;
	const unsigned int m_bufferLength;
	IDirect3DIndexBuffer9* m_pIndexBuffer;
};