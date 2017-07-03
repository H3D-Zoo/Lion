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
	return Set(d3dRenderState, dwValue);
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

#define DEF(x) m_pDevice->GetRenderState(x, &(m_RSValues[x]));
	DEF(D3DRS_TEXTUREFACTOR);
	DEF(D3DRS_FILLMODE);
	DEF(D3DRS_CULLMODE);
	DEF(D3DRS_SCISSORTESTENABLE);
	DEF(D3DRS_STENCILENABLE);
	DEF(D3DRS_STENCILREF);
	DEF(D3DRS_STENCILMASK);
	DEF(D3DRS_STENCILWRITEMASK);
	DEF(D3DRS_TWOSIDEDSTENCILMODE);
	DEF(D3DRS_STENCILFUNC);
	DEF(D3DRS_STENCILFAIL);
	DEF(D3DRS_STENCILZFAIL);
	DEF(D3DRS_STENCILPASS);
	DEF(D3DRS_CCW_STENCILFUNC);
	DEF(D3DRS_CCW_STENCILFAIL);
	DEF(D3DRS_CCW_STENCILZFAIL);
	DEF(D3DRS_CCW_STENCILPASS);
	DEF(D3DRS_ZENABLE);
	DEF(D3DRS_ZWRITEENABLE);
	DEF(D3DRS_ZFUNC);
	DEF(D3DRS_DEPTHBIAS);
	DEF(D3DRS_ALPHATESTENABLE);
	DEF(D3DRS_ALPHAFUNC);
	DEF(D3DRS_ALPHAREF);
	DEF(D3DRS_ALPHABLENDENABLE);
	DEF(D3DRS_SEPARATEALPHABLENDENABLE);
	DEF(D3DRS_BLENDOP);
	DEF(D3DRS_SRCBLEND);
	DEF(D3DRS_DESTBLEND);
	DEF(D3DRS_BLENDOPALPHA);
	DEF(D3DRS_SRCBLENDALPHA);
	DEF(D3DRS_DESTBLENDALPHA);
#undef DEF
}

StateManager::~StateManager()
{
	m_pDevice->Release();
}

HRESULT StateManager::Set(D3DRENDERSTATETYPE type, DWORD value)
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
