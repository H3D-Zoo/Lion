#include "APIContext.h"

void APIContext::AddRef()
{
	++m_refCount;
}

void APIContext::Release()
{

	if (m_refCount.OnlyOne())
	{
		delete this;
	}
	else
	{
		--m_refCount;
	}
}
