#include "FXEffect.h"
#include "Texture2D.h"
#include "TextureCube.h"

FXEffect::FXEffect(RenderStatistic& renderStatic)
	: m_renderStatistic(renderStatic)
{

}

unsigned int FXEffect::Begin(bool saveState)
{
	m_renderStatistic.OnBeginFx();
	return 0;
}

bool FXEffect::BeginPass(unsigned int passIndex)
{
	m_renderStatistic.OnBeginPass();
	return true;
}

void FXEffect::End()
{

}

void FXEffect::EndPass()
{

}

void FXEffect::SetValidateTechnique()
{

}

const char* FXEffect::GetTechniqueName(RenderAPI::HEffectParam techniqueID)
{
	return "";
}

RenderAPI::HEffectParam FXEffect::GetTechniqueByName(const char * name)
{
	return 0;
}

RenderAPI::HEffectParam FXEffect::GetTechniqueByID(unsigned int index)
{
	return 0;
}

RenderAPI::HEffectParam FXEffect::GetParameterByName(const char* name)
{
	return 0;
}

RenderAPI::HEffectParam FXEffect::GetParameterByName(RenderAPI::HEffectParam parent, const char* name)
{
	return 0;
}

RenderAPI::HEffectParam FXEffect::GetParameterElement(RenderAPI::HEffectParam parent, unsigned int elementIndex)
{
	return 0;
}

bool FXEffect::SetTechniqueByName(const char* name)
{
	return true;
}

bool FXEffect::SetMatrix(const char * paramName, const float* matrix)
{
	return true;
}

bool FXEffect::SetMatrixTranspose(const char * paramName, const float* matrix)
{
	return true;
}

bool FXEffect::SetMatrixInArray(const char* paramName, const float* matrix, unsigned int index)
{
	return true;
}

bool FXEffect::SetMatrixTransposeInArray(const char* paramName, const float* matrix, unsigned int index)
{
	return true;
}

bool FXEffect::SetMatrixArray(const char* paramName, const float* matrices, unsigned int count)
{
	return true;
}

bool FXEffect::SetMatrixTransposeArray(const char* paramName, const float* matrices, unsigned int count)
{
	return true;
}

bool FXEffect::SetValue(const char* paramName, const void* pValue, unsigned int sizeInByte)
{
	m_renderStatistic.OnEffectSetValue(sizeInByte);
	return true;
}

bool FXEffect::SetValueInArray(const char* paramName, const void* pValue, unsigned int sizeInByte, unsigned int index)
{
	m_renderStatistic.OnEffectSetValue(sizeInByte);
	return true;
}

bool FXEffect::SetFloat(const char* paramName, float fValue)
{
	m_renderStatistic.OnEffectSetScalar();
	return true;
}

bool FXEffect::SetInt(const char * paramName, int iValue)
{
	m_renderStatistic.OnEffectSetScalar();
	return true;
}

bool FXEffect::SetTexture(const char* paramName, RenderAPI::Texture* texture)
{
	//在stateManager中会统计数量这里只统计大小
	m_renderStatistic.OnSetTextureOnlySize(texture);
	m_renderStatistic.OnEffectSetTexture();
	return true;
}

bool FXEffect::SetTechnique(RenderAPI::HEffectParam hParam)
{
	return true;
}

bool FXEffect::SetMatrix(RenderAPI::HEffectParam hParam, const float* matrix)
{
	m_renderStatistic.OnEffectSetMatrix(1);
	return true;
}

bool FXEffect::SetMatrixTranspose(RenderAPI::HEffectParam hParam, const float* matrix)
{
	m_renderStatistic.OnEffectSetMatrix(1);
	return true;
}

bool FXEffect::SetMatrixInArray(RenderAPI::HEffectParam hParam, const float* matrix, unsigned int index)
{
	m_renderStatistic.OnEffectSetMatrix(1);
	return true;
}

bool FXEffect::SetMatrixTransposeInArray(RenderAPI::HEffectParam hParam, const float * matrix, unsigned int index)
{
	m_renderStatistic.OnEffectSetMatrix(1);
	return true;
}

bool FXEffect::SetMatrixArray(RenderAPI::HEffectParam hParam, const float* matrices, unsigned int count)
{
	m_renderStatistic.OnEffectSetMatrix(count);
	return true;
}

bool FXEffect::SetMatrixTransposeArray(RenderAPI::HEffectParam hParam, const float* matrices, unsigned int count)
{
	m_renderStatistic.OnEffectSetMatrix(count);
	return true;
}

bool FXEffect::SetValue(RenderAPI::HEffectParam hParam, const void * pValue, unsigned int sizeInByte)
{
	m_renderStatistic.OnEffectSetValue(sizeInByte);
	return true;
}

bool FXEffect::SetValueInArray(RenderAPI::HEffectParam hParam, const void * pValue, unsigned int sizeInByte, unsigned int index)
{
	m_renderStatistic.OnEffectSetValue(sizeInByte);
	return true;
}

bool FXEffect::SetFloat(RenderAPI::HEffectParam hParam, float fValue)
{
	m_renderStatistic.OnEffectSetScalar();
	return true;
}

bool FXEffect::SetInt(RenderAPI::HEffectParam hParam, int iValue)
{
	m_renderStatistic.OnEffectSetScalar();
	return true;
}

bool FXEffect::SetTexture(RenderAPI::HEffectParam hParam, RenderAPI::Texture* texture)
{
	m_renderStatistic.OnSetTextureOnlySize(texture);
	m_renderStatistic.OnEffectSetTexture();
	return true;
}

void FXEffect::CommitChange()
{

}

void FXEffect::OnLostDevice()
{

}

void FXEffect::OnResetDevice()
{

}

unsigned int FXEffect::AddReference()
{
	return ++m_refCount;
}

void FXEffect::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

void FXEffect::SetTextureSampler(RenderAPI::HEffectParam hParam, unsigned int index, RenderAPI::TextureSampler sampler)
{

}