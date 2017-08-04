#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <vector>
#include "DX9Include.h"

class VertexBuffer : public RenderAPI::VertexBuffer
{
public:
	VertexBuffer(IDirect3DVertexBuffer9* vertexBuffer, RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::VertexElement* elements, unsigned int elementCount, bool recreateWhenDeviceLost);

	~VertexBuffer();

	virtual RenderAPI::ResourceUsage GetUsage() const;

	virtual unsigned int GetVertexCount() const;

	virtual unsigned int GetVertexStride() const;

	virtual unsigned int GetLength() const;

	virtual const RenderAPI::VertexElement* GetElementPtr() const;

	virtual unsigned int GetElementCount() const;

	virtual void* Lock(unsigned int offset, unsigned int lockLength, RenderAPI::LockOption lockOption);

	virtual void* DiscardLock();

	virtual void Unlock();
	
	virtual bool NeedRecreateWhenDeviceLost();

	virtual void Release();

	IDirect3DVertexBuffer9* GetBufferPtr();

private:
	RenderAPI::ResourceUsage m_usage;
	const bool m_recreateWhenDeviceLost;
	unsigned int m_vertexCount;
	unsigned int m_vertexStride;
	unsigned int m_bufferLength;
	const std::vector<RenderAPI::VertexElement> m_vertexElements;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
};