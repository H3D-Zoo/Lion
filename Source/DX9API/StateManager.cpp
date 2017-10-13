#include "StateManager.h"
#include "EnumMapping.h"
#include "Context.h"

FXStateManager::FXStateManager(IDirect3DDevice9 * pDevice, RenderStatistic& pRenderStatisticsData) : StateManager(pDevice, pRenderStatisticsData)
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
	return SetSS(dwStage, d3dSamplerState, dwValue);
}

HRESULT FXStateManager::SetTextureStageState(THIS_ DWORD dwStage, D3DTEXTURESTAGESTATETYPE d3dTextureStageState, DWORD dwValue)
{
	return SetTSS(dwStage, d3dTextureStageState, dwValue);
}

HRESULT FXStateManager::SetTexture(THIS_ DWORD dwStage, LPDIRECT3DBASETEXTURE9 pTexture)
{
	m_renderStatistic.OnSetTexture(NULL);
	return m_pDevice->SetTexture(dwStage, pTexture);
}

HRESULT FXStateManager::SetVertexShader(THIS_ LPDIRECT3DVERTEXSHADER9 pShader)
{
	m_renderStatistic.OnSetVertexShader();
	return m_pDevice->SetVertexShader(pShader);
}

HRESULT FXStateManager::SetPixelShader(THIS_ LPDIRECT3DPIXELSHADER9 pShader)
{
	m_renderStatistic.OnSetPixelShader();
	return m_pDevice->SetPixelShader(pShader);
}

HRESULT FXStateManager::SetFVF(THIS_ DWORD dwFVF)
{
	m_renderStatistic.OnSetCustomFVF();
	return m_pDevice->SetFVF(dwFVF);
}

HRESULT FXStateManager::SetTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix)
{
	m_renderStatistic.OnSetTransForm();
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


StateManager::StateManager(IDirect3DDevice9* pDevice, RenderStatistic& renderStatistic)
	: m_pDevice(pDevice)
	, m_RSValues(RenderStateCount)
	, m_notSupportMinAnisotropic(true)
	, m_notSupportMagAnisotropic(true)
	, m_renderStatistic(renderStatistic)
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

	//设置各种状态的默认值
	m_isClipplaneenable = m_RSValues[D3DRS_CLIPPLANEENABLE] == TRUE;
	m_blendState.IsEnable = m_RSValues[D3DRS_ALPHABLENDENABLE] == TRUE;
	m_blendState.SrcBlend = BlendFactorMapping(m_RSValues[D3DRS_SRCBLEND]);
	m_blendState.DstBlend = BlendFactorMapping(m_RSValues[D3DRS_DESTBLEND]);	
	m_blendState.BlendOp = BlendOperatorMapping(m_RSValues[D3DRS_BLENDOP]);

	m_alphaSeparateBlendState.IsEnable = m_RSValues[D3DRS_SEPARATEALPHABLENDENABLE] == TRUE;
	m_alphaSeparateBlendState.SrcBlend = BlendFactorMapping(m_RSValues[D3DRS_SRCBLENDALPHA]);
	m_alphaSeparateBlendState.DstBlend = BlendFactorMapping(m_RSValues[D3DRS_DESTBLENDALPHA]);
	m_blendState.BlendOp = BlendOperatorMapping(m_RSValues[D3DRS_BLENDOPALPHA]);
	

	m_alphaTestState.IsEnable = m_RSValues[D3DRS_ALPHATESTENABLE] == TRUE;
	m_alphaTestState.Function = CompareMethodMapping(m_RSValues[D3DRS_ALPHAFUNC]);
	m_alphaTestState.Reference = m_RSValues[D3DRS_ALPHAREF] & 0xFF;

	m_depthTestState.IsEnable = m_RSValues[D3DRS_ALPHATESTENABLE] == TRUE;
	m_depthTestState.Function = CompareMethodMapping(m_RSValues[D3DRS_ZFUNC]);

	m_stencilTestState.IsEnable = m_RSValues[D3DRS_STENCILENABLE] == TRUE;
	m_stencilTestState.Reference = m_RSValues[D3DRS_STENCILREF] & 0xFF;
	m_stencilTestState.TestMask = m_RSValues[D3DRS_STENCILMASK];
	m_stencilTestState.WriteMask = m_RSValues[D3DRS_STENCILWRITEMASK];

	m_stencilTestState.FrontSide.Function = CompareMethodMapping(m_RSValues[D3DRS_STENCILFUNC]);
	m_stencilTestState.FrontSide.AllPass = StencilOpMapping(m_RSValues[D3DRS_CCW_STENCILPASS]);
	m_stencilTestState.FrontSide.SPassZFail = StencilOpMapping(m_RSValues[D3DRS_STENCILZFAIL]);
	m_stencilTestState.FrontSide.SFail = StencilOpMapping(m_RSValues[D3DRS_STENCILFAIL]);

	m_stencilTestState.TwoSide = m_RSValues[D3DRS_TWOSIDEDSTENCILMODE] == TRUE;
	if (m_stencilTestState.TwoSide)
	{
		m_stencilTestState.BackSide.Function = CompareMethodMapping(m_RSValues[D3DRS_CCW_STENCILFUNC]);;
		m_stencilTestState.BackSide.AllPass = StencilOpMapping(m_RSValues[D3DRS_CCW_STENCILPASS]);
		m_stencilTestState.BackSide.SPassZFail = StencilOpMapping(m_RSValues[D3DRS_CCW_STENCILZFAIL]);
		m_stencilTestState.BackSide.SFail = StencilOpMapping(m_RSValues[D3DRS_CCW_STENCILFAIL]);
	}

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
		GetSS(i, D3DSAMP_ADDRESSW);
		GetSS(i, D3DSAMP_MAXANISOTROPY);
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
		m_renderStatistic.OnSetRenderState();
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
		m_renderStatistic.OnSetSetTextureStageState();
		return m_pDevice->SetTextureStageState(slot, type, value);
	}
	else
	{
		return S_OK;
	}
}

HRESULT StateManager::SetSS(unsigned int slot, D3DSAMPLERSTATETYPE type, DWORD value)
{
	if (value == D3DTEXF_ANISOTROPIC)
	{
		if (type == D3DSAMP_MINFILTER && m_notSupportMinAnisotropic)
		{
			value = D3DTEXF_LINEAR;
		}
		else if (type == D3DSAMP_MAGFILTER && m_notSupportMagAnisotropic)
		{
			value = D3DTEXF_LINEAR;
		}
		else if (type == D3DSAMP_MIPFILTER)
		{
			value = D3DTEXF_LINEAR;
		}
	}
	DWORD& cv = m_SSValues[slot][type];
	if (cv != value)
	{
		cv = value;
		m_renderStatistic.OnSetSamplerState();
		return m_pDevice->SetSamplerState(slot, type, value);
	}
	else
	{
		return S_OK;
	}
}

unsigned int s_cullMode[] =
{
	D3DCULL_NONE,
	D3DCULL_CW,
	D3DCULL_CCW,
};

void StateManager::SetCullMode(RenderAPI::CullMode cullMode)
{
	SetRS(D3DRS_CULLMODE, s_cullMode[cullMode]);
	m_cullMode = cullMode;
}

unsigned int s_fillMode[] =
{
	D3DFILL_SOLID,
	D3DFILL_WIREFRAME,
	D3DFILL_POINT,
};

void StateManager::SetFillMode(RenderAPI::FillMode fillMode)
{
	SetRS(D3DRS_FILLMODE, s_fillMode[fillMode]);
	m_fillMode = fillMode;
}

unsigned int s_blendOps[] =
{
	D3DBLENDOP_ADD,
	D3DBLENDOP_SUBTRACT,
	D3DBLENDOP_MIN,
	D3DBLENDOP_MAX,
};

unsigned int s_blendFactors[] =
{
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_INVDESTCOLOR,
	D3DBLEND_INVDESTALPHA,
};

unsigned int s_compareMethods[] =
{
	D3DCMP_NEVER,
	D3DCMP_ALWAYS,
	D3DCMP_EQUAL,
	D3DCMP_NOTEQUAL,
	D3DCMP_LESS,
	D3DCMP_LESSEQUAL,
	D3DCMP_GREATER,
	D3DCMP_GREATEREQUAL,
};

unsigned int s_stencilOps[] =
{
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_ZERO,
	D3DSTENCILOP_REPLACE,
	D3DSTENCILOP_INCRSAT,
	D3DSTENCILOP_DECRSAT,
	D3DSTENCILOP_INVERT,
	D3DSTENCILOP_INCR,
	D3DSTENCILOP_DECR,
};

void StateManager::SetStencilTest(bool enable)
{
	SetRS(D3DRS_STENCILENABLE, enable ? TRUE : FALSE);
	m_stencilTestState.IsEnable = enable;
}

void StateManager::SetStencilReference(unsigned int ref)
{
	SetRS(D3DRS_STENCILREF, ref);
	m_stencilTestState.Reference = ref;
}

void StateManager::SetStencilReadMask(unsigned int mask)
{
	SetRS(D3DRS_STENCILMASK, mask);
	m_stencilTestState.TestMask = mask;
}

void StateManager::SetStencilWriteMask(unsigned int mask)
{
	SetRS(D3DRS_STENCILWRITEMASK, mask);
	m_stencilTestState.WriteMask = mask;
}

void StateManager::SetStencilTwoFace(bool enable)
{
	SetRS(D3DRS_TWOSIDEDSTENCILMODE, enable ? TRUE : FALSE);
	m_stencilTestState.TwoSide = enable;
}

void StateManager::SetStencilFrontFunction(RenderAPI::CompareMethod func)
{
	SetRS(D3DRS_STENCILFUNC, s_compareMethods[func]);
	m_stencilTestState.FrontSide.Function = func;
}

void StateManager::SetStencilFrontSFail(RenderAPI::StencilOp sop)
{
	SetRS(D3DRS_STENCILFAIL, s_stencilOps[sop]);
	m_stencilTestState.FrontSide.SFail = sop;
}

void StateManager::SetStencilFrontSPassZFail(RenderAPI::StencilOp sop)
{
	SetRS(D3DRS_STENCILZFAIL, s_stencilOps[sop]);
	m_stencilTestState.FrontSide.SPassZFail = sop;
}

void StateManager::SetStencilFrontAllPass(RenderAPI::StencilOp sop)
{
	SetRS(D3DRS_STENCILPASS, s_stencilOps[sop]);
	m_stencilTestState.FrontSide.AllPass = sop;
}

void StateManager::SetStencilBackFunction(RenderAPI::CompareMethod func)
{
	SetRS(D3DRS_CCW_STENCILFUNC, s_compareMethods[func]);
	m_stencilTestState.BackSide.Function = func;
}

void StateManager::SetStencilBackSFail(RenderAPI::StencilOp sop)
{
	SetRS(D3DRS_CCW_STENCILFAIL, s_stencilOps[sop]);
	m_stencilTestState.BackSide.SFail = sop;
}

void StateManager::SetStencilBackSPassZFail(RenderAPI::StencilOp sop)
{
	SetRS(D3DRS_CCW_STENCILZFAIL, s_stencilOps[sop]);
	m_stencilTestState.BackSide.SPassZFail = sop;
}

void StateManager::SetStencilBackAllPass(RenderAPI::StencilOp sop)
{
	SetRS(D3DRS_CCW_STENCILPASS, s_stencilOps[sop]);
	m_stencilTestState.BackSide.AllPass = sop;
}

void StateManager::SetDepthTest(bool enable)
{
	SetRS(D3DRS_ZENABLE, enable ? TRUE : FALSE); // it depend , True because we use auto stencilMap when create Device.
	m_depthTestState.IsEnable = enable;
}

void StateManager::SetDepthFunction(RenderAPI::CompareMethod func)
{
	SetRS(D3DRS_ZFUNC, s_compareMethods[func]);
	m_depthTestState.Function = func;
}

void StateManager::SetAlphaTest(bool enable)
{
	SetRS(D3DRS_ALPHATESTENABLE, enable ? TRUE : FALSE);
	m_alphaTestState.IsEnable = enable;
}

bool StateManager::IsScissorTestEnable() const
{
	return m_RSValues[D3DRS_SCISSORTESTENABLE] == TRUE;
}

void StateManager::SetAlphaFunction(RenderAPI::CompareMethod func)
{
	SetRS(D3DRS_ALPHAFUNC, s_compareMethods[func]);
	m_alphaTestState.Function = func;
}

void StateManager::SetAlphaReference(unsigned char ref)
{
	SetRS(D3DRS_ALPHAREF, ref);
	m_alphaTestState.Reference = ref;
}

void StateManager::SetAlphaBlending(bool enable)
{
	SetRS(D3DRS_ALPHABLENDENABLE, enable ? TRUE : FALSE);
	m_blendState.IsEnable = enable;
}

void StateManager::SetSeparateAlphaBlending(bool enable)
{
	SetRS(D3DRS_SEPARATEALPHABLENDENABLE, enable ? TRUE : FALSE);
	m_alphaSeparateBlendState.IsEnable = enable;
}
void StateManager::SetClipPlaneState(bool enable)
{
	SetRS(D3DRS_CLIPPLANEENABLE, enable ? TRUE : FALSE);
	m_isClipplaneenable = enable;
}


void StateManager::SetBlendingOp(RenderAPI::BlendOperator blendOp)
{
	SetRS(D3DRS_BLENDOP, s_blendOps[blendOp]);
	m_blendState.BlendOp = blendOp;
}

void StateManager::SetSrcBlending(RenderAPI::BlendFactor blendFactor)
{
	SetRS(D3DRS_SRCBLEND, s_blendFactors[blendFactor]);
	m_blendState.SrcBlend = blendFactor;
}

void StateManager::SetDstBlending(RenderAPI::BlendFactor blendFactor)
{
	SetRS(D3DRS_DESTBLEND, s_blendFactors[blendFactor]);
	m_blendState.DstBlend = blendFactor;
}

void StateManager::SetAlphaBlendingOp(RenderAPI::BlendOperator blendOp)
{
	SetRS(D3DRS_BLENDOPALPHA, s_blendOps[blendOp]);
	m_blendState.AlphaOp = blendOp;
}

void StateManager::SetAlphaSrcBlending(RenderAPI::BlendFactor blendFactor)
{
	SetRS(D3DRS_SRCBLENDALPHA, s_blendFactors[blendFactor]);
	m_alphaSeparateBlendState.SrcBlend = blendFactor;
}

void StateManager::SetAlphaDstBlending(RenderAPI::BlendFactor blendFactor)
{
	SetRS(D3DRS_DESTBLENDALPHA, s_blendFactors[blendFactor]);
	m_alphaSeparateBlendState.DstBlend = blendFactor;
}