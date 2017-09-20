#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include <map>
#include <vector>
#include <string>
#include "APIInstance.h"


class FXEffect : public RenderAPI::FXEffect
{
public:
	FXEffect(ID3DXEffect* pEffect);

	~FXEffect();

	virtual unsigned int Begin(bool saveState);

	virtual void End();

	virtual bool BeginPass(unsigned int passIndex);

	virtual void EndPass();

	virtual void SetValidateTechnique();

	virtual const char* GetTechniqueName(RenderAPI::HEffectParam);

	virtual RenderAPI::HEffectParam GetTechniqueByName(const char* name);

	virtual RenderAPI::HEffectParam GetTechniqueByID(unsigned int);

	virtual RenderAPI::HEffectParam GetParameterByName(const char* name);

	virtual RenderAPI::HEffectParam GetParameterByName(RenderAPI::HEffectParam parent, const char* name);

	virtual RenderAPI::HEffectParam GetParameterElement(RenderAPI::HEffectParam parent, unsigned int elementIndex);

	virtual bool SetTechniqueByName(const char* name);

	virtual bool SetMatrix(const char* paramName, const float* matrix);

	virtual bool SetMatrixTranspose(const char* paramName, const float* matrix);

	virtual bool SetMatrixInArray(const char* paramName, const float* matrix, unsigned int index);

	virtual bool SetMatrixTransposeInArray(const char* paramName, const float* matrix, unsigned int index);

	virtual bool SetMatrixArray(const char* paramName, const float* matrices, unsigned int count);

	virtual bool SetMatrixTransposeArray(const char* paramName, const float* matrices, unsigned int count);

	virtual bool SetValue(const char* paramName, const void* pValue, unsigned int sizeInByte);

	virtual bool SetValueInArray(const char* paramName, const void* pValue, unsigned int sizeInByte, unsigned int index);

	virtual bool SetFloat(const char* paramName, float fValue);

	virtual bool SetInt(const char* paramName, int iValue);

	virtual bool SetTexture(const char*  paramName, RenderAPI::Texture* texture);

	virtual bool SetTechnique(RenderAPI::HEffectParam hParam);

	virtual bool SetMatrix(RenderAPI::HEffectParam hParam, const float* matrix);

	virtual bool SetMatrixTranspose(RenderAPI::HEffectParam hParam, const float* matrix);

	virtual bool SetMatrixInArray(RenderAPI::HEffectParam hParam, const float* data, unsigned int index);

	virtual bool SetMatrixTransposeInArray(RenderAPI::HEffectParam hParam, const float* matrix, unsigned int index);

	virtual bool SetMatrixArray(RenderAPI::HEffectParam hParam, const float* matrices, unsigned int count);

	virtual bool SetMatrixTransposeArray(RenderAPI::HEffectParam hParam, const float* matrices, unsigned int count);

	virtual bool SetValue(RenderAPI::HEffectParam hParam, const void* pValue, unsigned int sizeInByte);

	virtual bool SetValueInArray(RenderAPI::HEffectParam hParam, const void* pValue, unsigned int sizeInByte, unsigned int index);

	virtual bool SetFloat(RenderAPI::HEffectParam hParam, float fValue);

	virtual bool SetInt(RenderAPI::HEffectParam hParam, int iValue);

	virtual bool SetTexture(RenderAPI::HEffectParam hParam, RenderAPI::Texture* texture);

	virtual void SetTextureSampler(RenderAPI::HEffectParam hParam, unsigned int index, RenderAPI::TextureSampler sampler);

	virtual void CommitChange();
	
	virtual void OnLostDevice();

	virtual void OnResetDevice();

	virtual void Release();

private:

	ID3DXEffect* m_pEffect;
	std::vector<D3DXHANDLE> m_hParamIDs;
	std::vector<D3DXHANDLE> m_hTechniqueIDs;
	std::map<std::string, unsigned int> m_hParamMapping;
	std::map<std::string, unsigned int> m_hTechniqueMapping;
};