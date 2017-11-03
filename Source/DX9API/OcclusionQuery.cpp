#include "OcclusionQuery.h"

OcclusionQuery::OcclusionQuery(IDirect3DQuery9 * pOcclusionQuery)
	: m_pOcclusionQuery(pOcclusionQuery)
{

}

OcclusionQuery::~OcclusionQuery()
{
	m_pOcclusionQuery->Release();
}

bool OcclusionQuery::Begin()
{
	return S_OK == m_pOcclusionQuery->Issue(D3DISSUE_BEGIN);
}

void OcclusionQuery::End()
{
	m_pOcclusionQuery->Issue(D3DISSUE_END);
}

bool OcclusionQuery::Get(void * dataPtr, unsigned int length)
{
	return S_OK == m_pOcclusionQuery->GetData(dataPtr, length, D3DGETDATA_FLUSH);
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
