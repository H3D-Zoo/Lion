#include "Context.h"
#include "APIInstance.h"
#include "InternalLogger.h"

RenderAPI::ContextLegacy * Context::GetContextLegacy()
{
	return this;
}
void Context::SetTexcoordTransform(unsigned int slot, RenderAPI::TextureTransform transform)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::ResetTexcoordIndex(unsigned int slot)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::DisableCustomShaderUsage()
{
	m_pDevice->SetVertexShader(NULL);
	m_pDevice->SetPixelShader(NULL);
}

void Context::SetWorldMatrix(const float * matrix)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::SetViewMatrix(const float * matrix)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::SetProjectionMatrix(const float * matrix)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::SetTextureMatrix(unsigned int slot, const float * matrix)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::SetFog(const RenderAPI::FogSetting & fog)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::SetLight(const RenderAPI::PointLight & pLight)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::SetLight(const RenderAPI::DirectionalLight & pLight)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::DisableLight()
{
	m_renderStateManager.LightEnable(0, FALSE);
	m_renderStateManager.SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}

void Context::SetMaterial(const RenderAPI::Material & mat)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::ProjectVertexPos(RenderAPI::Float3 & inoutPos, const float matMV[16], const float matP[16], RenderAPI::Viewport viewport)
{
	D3DXVECTOR3 vertOut;
	D3DXVECTOR3  vertIn;
	D3DVIEWPORT9 vp;
	D3DXMATRIX world, view, proj;
	D3DXMatrixIdentity(&world);

	vp.X = viewport.Left;
	vp.Y = viewport.Top;
	vp.Width = viewport.Width;
	vp.Height = viewport.Height;
	vp.MinZ = viewport.MinZ;
	vp.MaxZ = viewport.MaxZ;

	vertIn.x = inoutPos.x;
	vertIn.y = inoutPos.y;
	vertIn.z = inoutPos.z;

	D3DXVec3Project(&vertOut, &vertIn, &vp, (D3DXMATRIX*)matP, (D3DXMATRIX*)matMV, &world);
	inoutPos.x = vertOut.x;
	inoutPos.y = vertOut.y;
	inoutPos.z = vertOut.z;
}

void Context::SaveNXDebugRenderState()
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}

void Context::RestoreNXDebugRenderState(bool lightsOn)
{
	LOG_FUNCTION_D(*m_pAPI, "not supported.");
}
