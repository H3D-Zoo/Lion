#pragma once
#include "../../RenderAPI/RenderAPI.h"

class OcclusionQuery : public RenderAPI::OcclusionQuery
{
public:
	OcclusionQuery();

	~OcclusionQuery();

	virtual bool Begin();

	virtual void End();

	virtual bool Get(void* dataPtr, unsigned int length);

	virtual void Release();
};
