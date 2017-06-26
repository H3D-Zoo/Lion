#pragma once

#include "DX9Include.h"

class FXStateManager : public ID3DXEffectStateManager
{
protected:
	LONG m_lRef;
	WCHAR m_wszFrameStats[256];
	IDirect3DDevice9* m_pDevice;

public:
	FXStateManager(IDirect3DDevice9* pDevice) :
		m_lRef(1UL),
		m_pDevice(pDevice)
	{
		m_wszFrameStats[0] = 0;
	}

	virtual LPCWSTR EndFrameStats()
	{
		return m_wszFrameStats;
	}

	// methods inherited from ID3DXEffectStateManager
	STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv)
	{
		if (iid == IID_IUnknown || iid == IID_ID3DXEffectStateManager)
		{
			*ppv = this;
		}
		else
		{
			*ppv = NULL;
			return E_NOINTERFACE;
		}

		AddRef();
		return S_OK;
	}

	STDMETHOD_(ULONG, AddRef)(THIS)
	{
		return (ULONG)InterlockedIncrement(&m_lRef);
	}

	STDMETHOD_(ULONG, Release)(THIS)
	{
		if (0L == InterlockedDecrement(&m_lRef))
		{
			delete this;
			return 0L;
		}
		return m_lRef;
	}

	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE d3dRenderState, DWORD dwValue)
	{
		return m_pDevice->SetRenderState(d3dRenderState, dwValue);
	}

	STDMETHOD(SetSamplerState)(THIS_ DWORD dwStage, D3DSAMPLERSTATETYPE d3dSamplerState, DWORD dwValue)
	{
		return m_pDevice->SetSamplerState(dwStage, d3dSamplerState, dwValue);
	}

	STDMETHOD(SetTextureStageState)(THIS_ DWORD dwStage, D3DTEXTURESTAGESTATETYPE d3dTextureStageState, DWORD dwValue)
	{
		return m_pDevice->SetTextureStageState(dwStage, d3dTextureStageState, dwValue);
	}

	STDMETHOD(SetTexture)(THIS_ DWORD dwStage, LPDIRECT3DBASETEXTURE9 pTexture)
	{
		return m_pDevice->SetTexture(dwStage, pTexture);
	}

	STDMETHOD(SetVertexShader)(THIS_ LPDIRECT3DVERTEXSHADER9 pShader)
	{
		return m_pDevice->SetVertexShader(pShader);
	}

	STDMETHOD(SetPixelShader)(THIS_ LPDIRECT3DPIXELSHADER9 pShader)
	{
		return m_pDevice->SetPixelShader(pShader);
	}

	STDMETHOD(SetFVF)(THIS_ DWORD dwFVF)
	{
		return m_pDevice->SetFVF(dwFVF);
	}

	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
	{
		return m_pDevice->SetTransform(State, pMatrix);
	}

	STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9 *pMaterial)
	{
		return m_pDevice->SetMaterial(pMaterial);
	}

	STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9 *pLight)
	{
		return m_pDevice->SetLight(Index, pLight);
	}

	STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable)
	{
		return m_pDevice->LightEnable(Index, Enable);
	}

	STDMETHOD(SetNPatchMode)(THIS_ FLOAT NumSegments)
	{
		return m_pDevice->SetNPatchMode(NumSegments);
	}

	STDMETHOD(SetVertexShaderConstantF)(THIS_ unsigned int RegisterIndex, CONST FLOAT *pConstantData, unsigned int RegisterCount)
	{
		return m_pDevice->SetVertexShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
	}

	STDMETHOD(SetVertexShaderConstantI)(THIS_ unsigned int RegisterIndex, CONST INT *pConstantData, unsigned int RegisterCount)
	{
		return m_pDevice->SetVertexShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
	}

	STDMETHOD(SetVertexShaderConstantB)(THIS_ unsigned int RegisterIndex, CONST BOOL *pConstantData, unsigned int RegisterCount)
	{
		return m_pDevice->SetVertexShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
	}

	STDMETHOD(SetPixelShaderConstantF)(THIS_ unsigned int RegisterIndex, CONST FLOAT *pConstantData, unsigned int RegisterCount)
	{
		return m_pDevice->SetPixelShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
	}

	STDMETHOD(SetPixelShaderConstantI)(THIS_ unsigned int RegisterIndex, CONST INT *pConstantData, unsigned int RegisterCount)
	{
		return m_pDevice->SetPixelShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
	}

	STDMETHOD(SetPixelShaderConstantB)(THIS_ unsigned int RegisterIndex, CONST BOOL *pConstantData, unsigned int RegisterCount)
	{
		return m_pDevice->SetPixelShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
	}
};