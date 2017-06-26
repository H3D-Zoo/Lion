#include "FXEffect.h"

unsigned int FXEffect::Begin()
{
	return 0;
}

void FXEffect::End()
{

}

bool FXEffect::BeginPass(unsigned int passIndex)
{
	return true;
}

void FXEffect::EndPass()
{

}

void FXEffect::SetTechniqueByName(const char* name)
{

}

bool FXEffect::SetMatrix(const char* paramName, const float* matrix)
{
	return true;
}

bool FXEffect::SetMatrixInArray(const char* paramName, const float* data, unsigned int index)
{
	return true;
}

bool FXEffect::SetMatrixArray(const char* paramName, const float* data, unsigned int count)
{
	return true;
}

bool FXEffect::SetValue(const char* paramName, const void* pData, unsigned int sizeinByte)
{
	return true;
}

bool FXEffect::SetValueInArray(const char* paramName, const void* pData, unsigned int sizeinByte, unsigned int index)
{
	return true;
}

bool FXEffect::SetFloat(const char* paramName, float val)
{
	return true;
}

bool FXEffect::SetInt(const char* paramName, int data)
{
	return true;
}

bool FXEffect::SetTexture(const char*  paramName, RenderAPI::Texture2D* texture)
{
	return true;
}

void FXEffect::CommitChange()
{

}

void FXEffect::Release()
{
	delete this;
}
