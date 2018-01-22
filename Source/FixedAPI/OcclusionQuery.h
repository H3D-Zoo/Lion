#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"
#include "RefCount.hpp"
class OcclusionQuery : public RenderAPI::OcclusionQuery
{
public:
	OcclusionQuery(IDirect3DQuery9*);

	~OcclusionQuery();

	virtual bool Begin();

	virtual void End();

	virtual bool Get(void* dataPtr, unsigned int length);

	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
	IDirect3DQuery9* m_pOcclusionQuery;
};
