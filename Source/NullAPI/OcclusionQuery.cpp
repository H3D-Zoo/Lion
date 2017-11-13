#include "OcclusionQuery.h"

bool OcclusionQuery::Begin()
{
	return true;
}

void OcclusionQuery::End()
{

}

bool OcclusionQuery::Get(void * dataPtr, unsigned int length)
{
	memset(dataPtr, 0, length);
	return true;
}

unsigned int OcclusionQuery::AddReference()
{
	return ++m_refCount;
}

void OcclusionQuery::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}
