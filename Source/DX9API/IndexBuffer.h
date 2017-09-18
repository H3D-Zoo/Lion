#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "APIInstance.h"
#include "DX9Include.h"

class IndexBuffer : public RenderAPI::IndexBuffer
{
public:
	IndexBuffer(APIInstance* pAPI, IDirect3DIndexBuffer9* indexBuffer, RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int count, bool recreateWhenDeviceLost);

	~IndexBuffer();

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual RenderAPI::IndexFormat GetFormat() const;

	virtual unsigned int GetIndexCount() const;

	virtual unsigned int GetLength() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* DiscardLock();

	virtual void Unlock();

	virtual void Release();

	virtual bool NeedRecreateWhenDeviceLost() const;

	IDirect3DIndexBuffer9* GetD3DIndexBuffer();

private:
	APIInstance* m_pAPIInstance;
	RenderAPI::ResourceUsage m_usage;
	RenderAPI::IndexFormat m_indexFormat;
	const bool m_recreateWhenDeviceLost;
	unsigned int m_indexCount;
	unsigned int m_bufferLength;
	IDirect3DIndexBuffer9* m_pIndexBuffer;
};