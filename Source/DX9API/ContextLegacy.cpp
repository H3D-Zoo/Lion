#include "Context.h"
namespace
{
	DWORD s_d3dFogTableMode[] =
	{
		D3DFOG_LINEAR,
		D3DFOG_EXP,
		D3DFOG_EXP2
	};
}

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

void Context::SetFog(const RenderAPI::FogSetting & fog)
{
	if (fog.IsEnable)
	{
		m_renderStateManager.SetFog(TRUE);
		m_renderStateManager.SetFogTableMode(s_d3dFogTableMode[fog.TableMode]);
		m_renderStateManager.SetFogColor(fog.FogColor);

		if (fog.TableMode == RenderAPI::FOG_Linear)
		{
			DWORD* start = (DWORD*)&(fog.StartDepth);
			DWORD* end = (DWORD*)&(fog.EndDepth);
			m_renderStateManager.SetFogStartDepth(*start);
			m_renderStateManager.SetFogEndDepth(*end);
		}
		else
		{
			DWORD* density = (DWORD*)&(fog.FogDensity);
			m_renderStateManager.SetFogDensity(*density);
		}
	}
	else
	{
		m_renderStateManager.SetFog(FALSE);
	}
}