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

void Context::GenPerspectiveMatrixRH(float outMatrix[16], float fovRadian, float aspect, float nearZ, float farZ)
{

}

void Context::GenOrthoCenterMatrixRH(float outMatrix[16], float width, float height, float nearZ, float farZ)
{

}

void Context::GenOrthoOffCenterMatrixRH(float outMatrix[16], float left, float right, float bottom, float top, float nearZ, float farZ)
{

}

void Context::GenViewMatrix(float outMatrix[16], const RenderAPI::Float3 & eyePos, const RenderAPI::Float3 & lookAt, const RenderAPI::Float3 & upward)
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
