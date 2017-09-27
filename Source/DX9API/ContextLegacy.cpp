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

void Context::SetLight(const RenderAPI::PointLight & pLight)
{
	if (!pLight.IsEnable)
	{
		DisableLight();
	}
	else
	{
		D3DLIGHT9 light;
		memset(&light, 0, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_POINT;
		light.Diffuse.r = pLight.Diffuse.x;
		light.Diffuse.g = pLight.Diffuse.y;
		light.Diffuse.b = pLight.Diffuse.z;
		light.Diffuse.a = 1;
		light.Ambient.r = pLight.Ambient.x;
		light.Ambient.g = pLight.Ambient.y;
		light.Ambient.b = pLight.Ambient.z;
		light.Ambient.a = 1;

		//by sssa2000 090327
		//给dx7加上高光
		light.Specular.r = pLight.Specular.x;
		light.Specular.g = pLight.Specular.y;
		light.Specular.b = pLight.Specular.z;
		light.Specular.a = 1;
		light.Position.x = pLight.Position.x;
		light.Position.y = pLight.Position.y;
		light.Position.z = pLight.Position.z;
		light.Falloff = 0.0f;
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0;
		light.Attenuation2 = 0;
		light.Range = pLight.Range;
		m_renderStateManager.SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_renderStateManager.LightEnable(0, TRUE);
		m_renderStateManager.SetLight(0, &light);
	}
	
}

void Context::SetLight(const RenderAPI::DirectionalLight & pLight)
{
	if (!pLight.IsEnable)
	{
		DisableLight();
	}
	else
	{
		D3DLIGHT9 light;
		memset(&light, 0, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = pLight.Diffuse.x;
		light.Diffuse.g = pLight.Diffuse.y;
		light.Diffuse.b = pLight.Diffuse.z;
		light.Diffuse.a = 1;
		light.Ambient.r = pLight.Ambient.x;
		light.Ambient.g = pLight.Ambient.y;
		light.Ambient.b = pLight.Ambient.z;
		light.Ambient.a = 1;

		//by sssa2000 090327
		//给dx7加上高光
		light.Specular.r = pLight.Specular.x;
		light.Specular.g = pLight.Specular.y;
		light.Specular.b = pLight.Specular.z;
		light.Specular.a = 1;
		light.Direction.x = pLight.Direction.x;
		light.Direction.y = pLight.Direction.y;
		light.Direction.z = pLight.Direction.z;
		light.Falloff = 0.0f;
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0;
		light.Attenuation2 = 0;
		m_renderStateManager.SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		m_renderStateManager.LightEnable(0, TRUE);
		m_renderStateManager.SetLight(0, &light);
		
	}
}

void Context::DisableLight()
{
	m_renderStateManager.LightEnable(0, FALSE);
	m_renderStateManager.SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}

void Context::SetMaterial(const RenderAPI::Material & mat)
{

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mat.Diffuse.x;
	mtrl.Diffuse.g = mat.Diffuse.y;
	mtrl.Diffuse.b = mat.Diffuse.z;
	mtrl.Diffuse.a = mat.DiffuseA;
	mtrl.Ambient.r = mat.Ambient.x;
	mtrl.Ambient.g = mat.Ambient.y;
	mtrl.Ambient.b = mat.Ambient.z;
	mtrl.Ambient.a = mat.AmbientA;
	mtrl.Specular.r = mat.Specular.x;
	mtrl.Specular.g = mat.Specular.y;
	mtrl.Specular.b = mat.Specular.z;
	mtrl.Specular.a = mat.SpecularA;
	mtrl.Power = mat.Power;
	m_renderStateManager.SetMaterial(&mtrl);

}
