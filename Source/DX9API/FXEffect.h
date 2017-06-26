#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"

class FXEffect : public RenderAPI::FXEffect
{
public:
	FXEffect(ID3DXEffect* pEffect);

	~FXEffect();

	virtual unsigned int Begin();

	virtual void End();

	virtual bool BeginPass(unsigned int passIndex);

	virtual void EndPass();

	virtual void SetTechniqueByName(const char* name);

	virtual bool SetMatrix(const char* paramName, const float* matrix);

	virtual bool SetMatrixInArray(const char* paramName, const float* data, unsigned int index);

	virtual bool SetMatrixArray(const char* paramName, const float* data, unsigned int count);

	virtual bool SetValue(const char* paramName, const void* pData, unsigned int sizeinByte);

	virtual bool SetValueInArray(const char* paramName, const void* pData, unsigned int sizeinByte, unsigned int index);

	virtual bool SetFloat(const char* paramName, float val);

	virtual bool SetInt(const char* paramName, int data);

	virtual bool SetTexture(const char*  paramName, RenderAPI::Texture2D* texture);

	virtual void CommitChange();

	virtual void Release();

private:
	ID3DXEffect* m_pEffect;
};