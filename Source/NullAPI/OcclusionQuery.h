#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"

class OcclusionQuery : public RenderAPI::OcclusionQuery
{
public:
	virtual bool Begin();

	virtual void End();

	virtual bool Get(void* dataPtr, unsigned int length);

	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
};
