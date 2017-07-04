#include "APIContext.h"

void APIContext::AddRef()
{
	++m_refCount;
}

void APIContext::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}
