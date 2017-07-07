#include "StateManager.h"


FXStateManager::FXStateManager(IDirect3DDevice9 * pDevice) : StateManager(pDevice)
{
	m_wszFrameStats[0] = 0;
}

inline LPCWSTR FXStateManager::EndFrameStats()
{
	return m_wszFrameStats;
}

HRESULT FXStateManager::QueryInterface(THIS_ REFIID iid, LPVOID *ppv)
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


ULONG FXStateManager::AddRef(THIS)
{
	return ++m_refCount;
}

ULONG FXStateManager::Release(THIS)
{
	if (0 == m_refCount.Count())
	{
		//error;
	}
	--m_refCount;
	return m_refCount.Count();
}

HRESULT FXStateManager::SetRenderState(THIS_ D3DRENDERSTATETYPE d3dRenderState, DWORD dwValue)
{
	return SetRS(d3dRenderState, dwValue);
}

HRESULT FXStateManager::SetSamplerState(THIS_ DWORD dwStage, D3DSAMPLERSTATETYPE d3dSamplerState, DWORD dwValue)
{
	return m_pDevice->SetSamplerState(dwStage, d3dSamplerState, dwValue);
}

HRESULT FXStateManager::SetTextureStageState(THIS_ DWORD dwStage, D3DTEXTURESTAGESTATETYPE d3dTextureStageState, DWORD dwValue)
{
	return m_pDevice->SetTextureStageState(dwStage, d3dTextureStageState, dwValue);
}

HRESULT FXStateManager::SetTexture(THIS_ DWORD dwStage, LPDIRECT3DBASETEXTURE9 pTexture)
{
	return m_pDevice->SetTexture(dwStage, pTexture);
}

HRESULT FXStateManager::SetVertexShader(THIS_ LPDIRECT3DVERTEXSHADER9 pShader)
{
	return m_pDevice->SetVertexShader(pShader);
}

HRESULT FXStateManager::SetPixelShader(THIS_ LPDIRECT3DPIXELSHADER9 pShader)
{
	return m_pDevice->SetPixelShader(pShader);
}

HRESULT FXStateManager::SetFVF(THIS_ DWORD dwFVF)
{
	return m_pDevice->SetFVF(dwFVF);
}

HRESULT FXStateManager::SetTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
{
	return m_pDevice->SetTransform(State, pMatrix);
}

HRESULT FXStateManager::SetMaterial(THIS_ CONST D3DMATERIAL9 *pMaterial)
{
	return m_pDevice->SetMaterial(pMaterial);
}

HRESULT FXStateManager::SetLight(THIS_ DWORD Index, CONST D3DLIGHT9 *pLight)
{
	return m_pDevice->SetLight(Index, pLight);
}

HRESULT FXStateManager::LightEnable(THIS_ DWORD Index, BOOL Enable)
{
	return m_pDevice->LightEnable(Index, Enable);
}

HRESULT FXStateManager::SetNPatchMode(THIS_ FLOAT NumSegments)
{
	return m_pDevice->SetNPatchMode(NumSegments);
}

HRESULT FXStateManager::SetVertexShaderConstantF(THIS_ unsigned int RegisterIndex, CONST FLOAT *pConstantData, unsigned int RegisterCount)
{
	return m_pDevice->SetVertexShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
}

HRESULT FXStateManager::SetVertexShaderConstantI(THIS_ unsigned int RegisterIndex, CONST INT *pConstantData, unsigned int RegisterCount)
{
	return m_pDevice->SetVertexShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
}

HRESULT FXStateManager::SetVertexShaderConstantB(THIS_ unsigned int RegisterIndex, CONST BOOL *pConstantData, unsigned int RegisterCount)
{
	return m_pDevice->SetVertexShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
}

HRESULT FXStateManager::SetPixelShaderConstantF(THIS_ unsigned int RegisterIndex, CONST FLOAT *pConstantData, unsigned int RegisterCount)
{
	return m_pDevice->SetPixelShaderConstantF(RegisterIndex, pConstantData, RegisterCount);
}

HRESULT FXStateManager::SetPixelShaderConstantI(THIS_ unsigned int RegisterIndex, CONST INT *pConstantData, unsigned int RegisterCount)
{
	return m_pDevice->SetPixelShaderConstantI(RegisterIndex, pConstantData, RegisterCount);
}

HRESULT FXStateManager::SetPixelShaderConstantB(THIS_ unsigned int RegisterIndex, CONST BOOL *pConstantData, unsigned int RegisterCount)
{
	return m_pDevice->SetPixelShaderConstantB(RegisterIndex, pConstantData, RegisterCount);
}


StateManager::StateManager(IDirect3DDevice9* pDevice)
	: m_pDevice(pDevice)
	, m_RSValues(RenderStateCount)
{
	pDevice->AddRef();
	for (int i = 0; i < TextureSlotCount; i++)
	{
		m_TSSValues[i].resize(TextureStageStateCount);
		m_SSValues[i].resize(SamplerStateCount);
	}

#define GetRS(x) m_pDevice->GetRenderState(x, &(m_RSValues[x]));

	GetRS(D3DRS_TEXTUREFACTOR);
	GetRS(D3DRS_FILLMODE);
	GetRS(D3DRS_CULLMODE);
	GetRS(D3DRS_SCISSORTESTENABLE);
	GetRS(D3DRS_STENCILENABLE);
	GetRS(D3DRS_STENCILREF);
	GetRS(D3DRS_STENCILMASK);
	GetRS(D3DRS_STENCILWRITEMASK);
	GetRS(D3DRS_TWOSIDEDSTENCILMODE);
	GetRS(D3DRS_STENCILFUNC);
	GetRS(D3DRS_STENCILFAIL);
	GetRS(D3DRS_STENCILZFAIL);
	GetRS(D3DRS_STENCILPASS);
	GetRS(D3DRS_CCW_STENCILFUNC);
	GetRS(D3DRS_CCW_STENCILFAIL);
	GetRS(D3DRS_CCW_STENCILZFAIL);
	GetRS(D3DRS_CCW_STENCILPASS);
	GetRS(D3DRS_ZENABLE);
	GetRS(D3DRS_ZWRITEENABLE);
	GetRS(D3DRS_ZFUNC);
	GetRS(D3DRS_DEPTHBIAS);
	GetRS(D3DRS_ALPHATESTENABLE);
	GetRS(D3DRS_ALPHAFUNC);
	GetRS(D3DRS_ALPHAREF);
	GetRS(D3DRS_ALPHABLENDENABLE);
	GetRS(D3DRS_SEPARATEALPHABLENDENABLE);
	GetRS(D3DRS_BLENDOP);
	GetRS(D3DRS_SRCBLEND);
	GetRS(D3DRS_DESTBLEND);
	GetRS(D3DRS_BLENDOPALPHA);
	GetRS(D3DRS_SRCBLENDALPHA);
	GetRS(D3DRS_DESTBLENDALPHA);
#undef GetRS

#define GetTSS(slot, x) m_pDevice->GetTextureStageState(slot, x, &(m_TSSValues[slot][x]));
#define GetSS(slot, x) m_pDevice->GetSamplerState(slot, x, &(m_SSValues[slot][x]));

	for (int i = 0; i < TextureSlotCount; i++)
	{
		GetTSS(i, D3DTSS_COLOROP);
		GetTSS(i, D3DTSS_COLORARG1);
		GetTSS(i, D3DTSS_COLORARG2);
		GetTSS(i, D3DTSS_ALPHAOP);
		GetTSS(i, D3DTSS_ALPHAARG1);
		GetTSS(i, D3DTSS_ALPHAARG2);

		GetSS(i, D3DSAMP_MINFILTER);
		GetSS(i, D3DSAMP_MAGFILTER);
		GetSS(i, D3DSAMP_MIPFILTER);
		GetSS(i, D3DSAMP_ADDRESSU);
		GetSS(i, D3DSAMP_ADDRESSV);
	}
#undef GetTSS
#undef GetSS
}

StateManager::~StateManager()
{
	m_pDevice->Release();
}

HRESULT StateManager::SetRS(D3DRENDERSTATETYPE type, DWORD value)
{
	DWORD& cv = m_RSValues[type];
	if (cv != value)
	{
		cv = value;
		return m_pDevice->SetRenderState(type, value);
	}
	else
	{
		return S_OK;
	}
}

HRESULT StateManager::SetTSS(unsigned int slot, D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
	DWORD& cv = m_TSSValues[slot][type];
	if (cv != value)
	{
		cv = value;
		return m_pDevice->SetTextureStageState(slot, type, value);
	}
	else
	{
		return S_OK;
	}
}

HRESULT StateManager::SetSS(unsigned int slot, D3DSAMPLERSTATETYPE type, DWORD value)
{
	DWORD& cv = m_SSValues[slot][type];
	if (cv != value)
	{
		cv = value;
		return m_pDevice->SetSamplerState(slot, type, value);
	}
	else
	{
		return S_OK;
	}
}
