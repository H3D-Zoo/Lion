#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"

class OcclusionQuery : public RenderAPI::OcclusionQuery
{
public:
	OcclusionQuery(IDirect3DQuery9* pOcclusionQuery);

	~OcclusionQuery();

	virtual bool Begin();

	virtual void End();

	virtual bool Get(void* dataPtr, unsigned int length);

	virtual void Release();

private:
	IDirect3DQuery9* m_pOcclusionQuery;
};
