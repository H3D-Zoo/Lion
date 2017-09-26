#include "Context.h"

RenderAPI::ContextLegacy * Context::GetContextLegacy()
{
	return this;
}

void Context::DisableTexcoordTransform(unsigned int slot)
{
	m_renderStateManager.SetTextureStageState(slot, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
}

void Context::ResetTexcoordIndex(unsigned int slot)
{
	m_renderStateManager.SetTextureStageState(slot, D3DTSS_TEXCOORDINDEX, slot);
}

void Context::DisableCustomShaderUsage()
{
	m_pDevice->SetVertexShader(NULL);
	m_pDevice->SetPixelShader(NULL);
}

void Context::SetCustomFVF(unsigned int fvf)
{
	m_pDevice->SetFVF(fvf);
}

void Context::SetWorldMatrix(const float * matrix)
{
	m_pDevice->SetTransform(D3DTS_WORLD, (const D3DMATRIX*)matrix);
}

void Context::SetViewMatrix(const float * matrix)
{
	m_pDevice->SetTransform(D3DTS_VIEW, (const D3DMATRIX*)matrix);
}

void Context::SetProjectionMatrix(const float * matrix)
{
	m_pDevice->SetTransform(D3DTS_PROJECTION, (const D3DMATRIX*)matrix);
}

void Context::SetTextureMatrix(unsigned int slot, const float * matrix)
{
	if (slot < 8)
	{	
		m_pDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + slot), (const D3DMATRIX*)matrix);
	}
}
