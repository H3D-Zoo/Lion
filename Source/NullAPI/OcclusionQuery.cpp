#include "OcclusionQuery.h"

OcclusionQuery::OcclusionQuery()
{

}

OcclusionQuery::~OcclusionQuery()
{
}

bool OcclusionQuery::Begin()
{
	return true;
}

void OcclusionQuery::End()
{

}

bool OcclusionQuery::Get(void * dataPtr, unsigned int length)
{
	return false;
}

void OcclusionQuery::Release()
{
	delete this;
}
