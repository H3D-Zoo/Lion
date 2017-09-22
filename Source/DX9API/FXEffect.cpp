#include "FXEffect.h"
#include "Texture2D.h"
#include "TextureCube.h"


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

unsigned int FXEffect::Begin(bool saveState)
{
	UINT passCount = 0;
	if (FAILED(m_pEffect->Begin(&passCount, saveState ? 0 : D3DXFX_DONOTSAVESTATE)))
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

const char* FXEffect::GetTechniqueName(RenderAPI::HEffectParam techniqueID)
{
	D3DXTECHNIQUE_DESC pDesc;
	if (S_OK == m_pEffect->GetTechniqueDesc(m_hTechniqueIDs[techniqueID], &pDesc))
	{
		return pDesc.Name;
	}
	else
	{
		return "";
	}
}

RenderAPI::HEffectParam FXEffect::GetTechniqueByName(const char * name)
{
	std::string nameStr = name;
	if (m_hTechniqueMapping.count(nameStr) == 0)
	{
		D3DXHANDLE h = m_pEffect->GetTechniqueByName(name);
		if (h == NULL) return RenderAPI::hInvalidParam;

		RenderAPI::HEffectParam rst = (RenderAPI::HEffectParam)m_hTechniqueIDs.size();
		m_hTechniqueIDs.push_back(h);
		m_hTechniqueMapping.insert(std::pair<std::string, unsigned int>(nameStr, rst));
		return rst;
	}
	else
	{
		return m_hTechniqueMapping[nameStr];
	}
}

RenderAPI::HEffectParam FXEffect::GetTechniqueByID(unsigned int index)
{
	D3DXHANDLE h = m_pEffect->GetTechnique(index);
	if (h == NULL) return RenderAPI::hInvalidParam;

	size_t n = m_hTechniqueIDs.size();
	for (size_t i = 0; i < n; i++)
	{
		if (m_hTechniqueIDs[i] == h)
		{
			return i;
		}
	}

	RenderAPI::HEffectParam rst = (RenderAPI::HEffectParam)m_hTechniqueIDs.size();
	m_hTechniqueIDs.push_back(h);
	return rst;
}

RenderAPI::HEffectParam FXEffect::GetParameterByName(const char* name)
{
	std::string nameStr = name;
	if (m_hParamMapping.count(nameStr) == 0)
	{
		D3DXHANDLE h = m_pEffect->GetParameterByName(NULL, name);
		if (h == NULL) return RenderAPI::hInvalidParam;

		RenderAPI::HEffectParam rst = (RenderAPI::HEffectParam)m_hParamIDs.size();
		m_hParamIDs.push_back(h);
		m_hParamMapping.insert(std::pair<std::string, unsigned int>(nameStr, rst));
		return rst;
	}
	else
	{
		return m_hParamMapping[nameStr];
	}
}

RenderAPI::HEffectParam FXEffect::GetParameterByName(RenderAPI::HEffectParam parent, const char* name)
{
	if (parent >= m_hParamIDs.size())
		return RenderAPI::hInvalidParam;

	std::string nameStr = name;
	if (m_hParamMapping.count(nameStr) == 0)
	{
		D3DXHANDLE h = m_pEffect->GetParameterByName(m_hParamIDs[parent], name);
		if (h == NULL) return RenderAPI::hInvalidParam;

		RenderAPI::HEffectParam rst = (RenderAPI::HEffectParam)m_hParamIDs.size();
		m_hParamIDs.push_back(h);
		m_hParamMapping.insert(std::pair<std::string, unsigned int>(nameStr, rst));
		return rst;
	}
	else
	{
		return m_hParamMapping[nameStr];
	}

}

RenderAPI::HEffectParam FXEffect::GetParameterElement(RenderAPI::HEffectParam parent, unsigned int elementIndex)
{
	if (parent >= m_hParamIDs.size())
		return RenderAPI::hInvalidParam;

	D3DXHANDLE h = m_pEffect->GetParameterElement(m_hParamIDs[parent], elementIndex);
	if (h == NULL) return RenderAPI::hInvalidParam;

	size_t n = m_hParamIDs.size();
	for (size_t i = 0; i < n; i++)
	{
		if (m_hParamIDs[i] == h)
		{
			return i;
		}
	}

	RenderAPI::HEffectParam rst = (RenderAPI::HEffectParam)m_hParamIDs.size();
	m_hParamIDs.push_back(h);
	return rst;
}

bool FXEffect::SetTechniqueByName(const char* name)
{
	return S_OK == m_pEffect->SetTechnique(name);
}

bool FXEffect::SetMatrix(const char * paramName, const float* matrix)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetMatrix(handle, (const D3DXMATRIX*)matrix);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixTranspose(const char * paramName, const float* matrix)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetMatrixTranspose(handle, (const D3DXMATRIX*)matrix);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixInArray(const char* paramName, const float* matrix, unsigned int index)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(handle, index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetMatrix(subhandle, (const D3DXMATRIX*)matrix);
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

bool FXEffect::SetMatrixTransposeInArray(const char* paramName, const float* matrix, unsigned int index)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(handle, index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetMatrixTranspose(subhandle, (const D3DXMATRIX*)matrix);
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

bool FXEffect::SetMatrixArray(const char* paramName, const float* matrices, unsigned int count)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetMatrixArray(handle, (const D3DXMATRIX*)matrices, count);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixTransposeArray(const char* paramName, const float* matrices, unsigned int count)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetMatrixTransposeArray(handle, (const D3DXMATRIX*)matrices, count);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetValue(const char* paramName, const void* pValue, unsigned int sizeInByte)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetValue(handle, pValue, sizeInByte);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetValueInArray(const char* paramName, const void* pValue, unsigned int sizeInByte, unsigned int index)
{
	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(handle, index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetValue(subhandle, pValue, sizeInByte);
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

bool FXEffect::SetFloat(const char* paramName, float fValue)
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

bool FXEffect::SetTexture(const char* paramName, RenderAPI::Texture* texture)
{
	IDirect3DBaseTexture9* texturePtr = NULL;
	if (texture != NULL)
	{
		if (texture->IsCubemap())
			texturePtr = ((::TextureCube*)(texture))->GetD3DTexture();
		else
			texturePtr = ((::Texture2D*)(texture))->GetD3DTexture();
	}

	D3DXHANDLE handle = m_pEffect->GetParameterByName(NULL, paramName);
	if (handle != NULL)
	{
		return S_OK == m_pEffect->SetTexture(handle, texturePtr);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetTechnique(RenderAPI::HEffectParam hParam)
{
	if (hParam < m_hTechniqueIDs.size())
	{
		return S_OK == m_pEffect->SetTechnique(m_hTechniqueIDs[hParam]);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrix(RenderAPI::HEffectParam hParam, const float* matrix)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetMatrix(m_hParamIDs[hParam], (const D3DXMATRIX*)matrix);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixTranspose(RenderAPI::HEffectParam hParam, const float* matrix)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetMatrixTranspose(m_hParamIDs[hParam], (const D3DXMATRIX*)matrix);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixInArray(RenderAPI::HEffectParam hParam, const float* matrix, unsigned int index)
{
	if (hParam < m_hParamIDs.size())
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(m_hParamIDs[hParam], index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetMatrix(subhandle, (const D3DXMATRIX*)matrix);
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

bool FXEffect::SetMatrixTransposeInArray(RenderAPI::HEffectParam hParam, const float * matrix, unsigned int index)
{
	if (hParam < m_hParamIDs.size())
	{
		D3DXHANDLE subhandle = m_pEffect->GetParameterElement(m_hParamIDs[hParam], index);
		if (subhandle != NULL)
		{
			return S_OK == m_pEffect->SetMatrixTranspose(subhandle, (const D3DXMATRIX*)matrix);
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

bool FXEffect::SetMatrixArray(RenderAPI::HEffectParam hParam, const float* matrices, unsigned int count)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetMatrixArray(m_hParamIDs[hParam], (const D3DXMATRIX*)matrices, count);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetMatrixTransposeArray(RenderAPI::HEffectParam hParam, const float* matrices, unsigned int count)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetMatrixTransposeArray(m_hParamIDs[hParam], (const D3DXMATRIX*)matrices, count);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetValue(RenderAPI::HEffectParam hParam, const void * pValue, unsigned int sizeInByte)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetValue(m_hParamIDs[hParam], pValue, sizeInByte);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetValueInArray(RenderAPI::HEffectParam hParam, const void * pValue, unsigned int sizeInByte, unsigned int index)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetValue(m_hParamIDs[hParam], pValue, sizeInByte);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetFloat(RenderAPI::HEffectParam hParam, float fValue)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetFloat(m_hParamIDs[hParam], fValue);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetInt(RenderAPI::HEffectParam hParam, int iValue)
{
	if (hParam < m_hParamIDs.size())
	{
		return S_OK == m_pEffect->SetInt(m_hParamIDs[hParam], iValue);
	}
	else
	{
		return false;
	}
}

bool FXEffect::SetTexture(RenderAPI::HEffectParam hParam, RenderAPI::Texture* texture)
{
	if (hParam < m_hParamIDs.size())
	{
		IDirect3DTexture9* texturePtr = NULL;
		if (texture != NULL)
		{
			texturePtr = ((::Texture2D*)(texture))->GetD3DTexture();
		}
		return S_OK == m_pEffect->SetTexture(m_hParamIDs[hParam], texturePtr);
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

void FXEffect::OnLostDevice()
{
	m_pEffect->OnLostDevice();
}

void FXEffect::OnResetDevice()
{
	m_pEffect->OnResetDevice();
}


namespace
{
	D3DTEXTUREADDRESS s_textureAddress[] =
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_CLAMP,
		D3DTADDRESS_MIRROR,
		D3DTADDRESS_BORDER,
	};
	D3DTEXTUREFILTERTYPE s_d3dSamplerFilter[] =
	{
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
	};
}

void FXEffect::SetTextureSampler(RenderAPI::HEffectParam hParam, unsigned int index, RenderAPI::TextureSampler sampler)
{
	if (hParam >= m_hParamIDs.size())
		return;

	D3DXHANDLE hSampler = m_hParamIDs[hParam];
	D3DXHANDLE hAddressU = m_pEffect->GetParameterByName(hSampler, "AddressU");
	D3DXHANDLE hAddressV = m_pEffect->GetParameterByName(hSampler, "AddressV");
	D3DXHANDLE hAddressW = m_pEffect->GetParameterByName(hSampler, "AddressW");
	D3DXHANDLE hBorderColor = m_pEffect->GetParameterByName(hSampler, "BorderColor");
	D3DXHANDLE hMagFilter = m_pEffect->GetParameterByName(hSampler, "MagFilter");
	D3DXHANDLE hMinFilter = m_pEffect->GetParameterByName(hSampler, "MinFilter");
	D3DXHANDLE hMipFilter = m_pEffect->GetParameterByName(hSampler, "MipFilter");


	DWORD value;
	if (hAddressU != NULL)
	{
		value = s_textureAddress[sampler.AddressU];
		m_pEffect->SetValue(hAddressU, &value, sizeof(DWORD));
	}
	if (hAddressV != NULL)
	{
		value = s_textureAddress[sampler.AddressV];
		m_pEffect->SetValue(hAddressV, &value, sizeof(DWORD));
	}
	if (hAddressW != NULL)
	{
		value = s_textureAddress[sampler.AddressW];
		m_pEffect->SetValue(hAddressW, &value, sizeof(DWORD));
	}
	if (hMagFilter != NULL)
	{
		if (sampler.MagFilter == RenderAPI::FILTER_Anisotropic)
		{
			value = s_d3dSamplerFilter[RenderAPI::FILTER_Linear];
		}
		else
		{
			value = s_d3dSamplerFilter[sampler.MagFilter];
		}

		m_pEffect->SetValue(hMagFilter, &value, sizeof(DWORD));
	}

	if (hMinFilter != NULL)
	{
		if (sampler.MinFilter == RenderAPI::FILTER_Anisotropic)
		{
			bool anisotropy = false;
			if (sampler.OptionalAnisotropicFilter != RenderAPI::AA_Disable)
			{
				ID3DXEffectStateManager* pManager = NULL;
				m_pEffect->GetStateManager(&pManager);
				if (pManager != NULL)
				{
					anisotropy = true;
					value = D3DTEXF_ANISOTROPIC;
					DWORD maxanisotropies[] = { 0,2,4,6,8,10 };
					pManager->SetSamplerState(index, D3DSAMP_MAXANISOTROPY, maxanisotropies[sampler.OptionalAnisotropicFilter]);
				}
			}

			if (!anisotropy)
			{
				value = D3DTEXF_LINEAR;
			}
		}
		else
		{
			value = s_d3dSamplerFilter[sampler.MinFilter];
		}

		m_pEffect->SetValue(hMinFilter, &value, sizeof(DWORD));
	}

	if (hMipFilter != NULL)
	{
		value = s_d3dSamplerFilter[sampler.MipFilter];
		m_pEffect->SetValue(hMipFilter, &value, sizeof(DWORD));
	}

	if (hBorderColor != NULL)
	{
		value = sampler.BorderColor;
		m_pEffect->SetValue(hBorderColor, &value, sizeof(DWORD));
	}
}