#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "APIInstance.h"
#include "DX9Include.h"

class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(APIInstance* pAPIInstance, IDirect3DVertexBuffer9* vertexBuffer, RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, bool recreateWhenDeviceLost);

	~VertexBuffer();

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual unsigned int GetVertexCount() const;

	virtual unsigned int GetVertexStride() const;

	virtual unsigned int GetLength() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* DiscardLock();

	virtual void Unlock();
	
	virtual bool NeedRecreateWhenDeviceLost() const;

	virtual void Release();

	IDirect3DVertexBuffer9* GetBufferPtr();

private:
	APIInstance* m_pAPIInstance;
	RenderAPI::ResourceUsage m_usage;
	const bool m_recreateWhenDeviceLost;
	unsigned int m_vertexCount;
	unsigned int m_vertexStride;
	unsigned int m_bufferLength;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
};