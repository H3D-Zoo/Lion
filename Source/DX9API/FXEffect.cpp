#include "FXEffect.h"
#include "Texture2D.h"



FXEffect::FXEffect(ID3DXEffect * pEffect)
	: m_pEffect(pEffect)
{
}

FXEffect::~FXEffect()
{
	m_pEffect->Release();
	m_pEffect = NULL;
}

void FXEffect::Release()
{
	delete this;
}

unsigned int FXEffect::Begin()
{
	UINT passCount = 0;
	if (FAILED(m_pEffect->Begin(&passCount, D3DXFX_DONOTSAVESTATE)))
	{
		return 0;
	}
	else
	{
		return passCount;
	}
}

bool FXEffect::BeginPass(unsigned int passIndex)
{
	return SUCCEEDED(m_pEffect->BeginPass(passIndex));
}

void FXEffect::End()
{
	m_pEffect->End();
}

void FXEffect::EndPass()
{
	m_pEffect->EndPass();
}

void FXEffect::SetValidateTechnique()
{
	D3DXHANDLE hValidateTechnique;
	m_pEffect->FindNextValidTechnique(NULL, &hValidateTechnique);
	if (hValidateTechnique != NULL)
	{
		m_pEffect->SetTechnique(hValidateTechnique);
	}
}

void FXEffect::SetTechniqueByName(const char* name)
{
	m_pEffect->SetTechnique(name);
}

bool FXEffect::SetMatrix(const char * paramName, const float * matrix)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetMatrixTranspose(handle, (D3DXMATRIX*)matrix);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixInArray(const char * paramName, const float * matrix, unsigned int index)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(handle, index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetMatrixTranspose(subhandle, (const D3DXMATRIX*)&matrix);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixArray(const char * paramName, const float * matrix, unsigned int count)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetMatrixTransposeArray(handle, (D3DXMATRIX*)matrix, count);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetValue(const char * paramName, const void * pData, unsigned int sizeinByte)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetValue(handle, pData, sizeinByte);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetValueInArray(const char * paramName, const void * pData, unsigned int sizeinByte, unsigned int index)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(handle, index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetValue(subhandle, pData, sizeinByte);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetFloat(const char * paramName, float fValue)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetFloat(handle, fValue);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetInt(const char * paramName, int iValue)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetInt(handle, iValue);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetTexture(const char * paramName, RenderAPI::Texture2D * texture)
{
	IDirect3DTexture9* texturePtr = ((::Texture2D*)(texture))->GetD3DTexture();
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL || texturePtr != NULL)
	{
		return S_OK == m_pEffect->SetTexture(handle, texturePtr);
	}
	else
	{
		return false;
	}
}

void FXEffect::CommitChange()
{
	m_pEffect->CommitChanges();
}
