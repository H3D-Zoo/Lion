#include "Context.h"

RenderAPI::ContextLegacy * Context::GetContextLegacy()
{
	return this;
}

void Context::SetTexcoordTransform(unsigned int slot, RenderAPI::TextureTransform transform)
{
	
}

void Context::ResetTexcoordIndex(unsigned int slot)
{
	
}

void Context::DisableCustomShaderUsage()
{
	
}

void Context::SetWorldMatrix(const float * matrix)
{

}

void Context::SetViewMatrix(const float * matrix)
{

}

void Context::SetProjectionMatrix(const float * matrix)
{

}

void Context::SetTextureMatrix(unsigned int slot, const float * matrix)
{

}

void Context::SetFog(const RenderAPI::FogSetting & fog)
{

}

void Context::SetLight(const RenderAPI::PointLight & pLight)
{

}

void Context::SetLight(const RenderAPI::DirectionalLight & pLight)
{

}

void Context::DisableLight()
{

}

void Context::SetMaterial(const RenderAPI::Material & mat)
{
	
}

void Context::ProjectVertexPos(RenderAPI::Float3 & inoutPos, const float matMV[16], const float matP[16], RenderAPI::Viewport viewport)
{

}

void Context::SaveNXDebugRenderState()
{

}

void Context::RestoreNXDebugRenderState(bool lightsOn)
{

}
