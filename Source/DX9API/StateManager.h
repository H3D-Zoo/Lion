#pragma once
#include <vector>
#include "DX9Include.h"
#include "RefCount.hpp"

#define RSDefine(name, rs) void Set##name(DWORD value) { SetRS(rs, value); }
#define TSSDefine(name, rs) void Set##name(unsigned int slot, DWORD value) { SetTSS(slot,rs, value); }
#define SSDefine(name, rs) void Set##name(unsigned int slot, DWORD value) { SetSS(slot,rs, value); }

class StateManager
{
public:

	StateManager(IDirect3DDevice9* pDevice);

	~StateManager();

	HRESULT SetRS(D3DRENDERSTATETYPE type, DWORD value);

	HRESULT SetTSS(unsigned int slot, D3DTEXTURESTAGESTATETYPE type, DWORD value);

	HRESULT SetSS(unsigned int slot, D3DSAMPLERSTATETYPE type, DWORD value);

	RSDefine(TextureFactor, D3DRS_TEXTUREFACTOR);

	RSDefine(FillMode, D3DRS_FILLMODE);

	RSDefine(CullMode, D3DRS_CULLMODE);

	RSDefine(ScissorTest, D3DRS_SCISSORTESTENABLE);

	RSDefine(StencilTest, D3DRS_STENCILENABLE);

	RSDefine(StencilReference, D3DRS_STENCILREF);

	RSDefine(StencilReadMask, D3DRS_STENCILMASK);

	RSDefine(StencilWriteMask, D3DRS_STENCILWRITEMASK);

	RSDefine(StencilTwoFace, D3DRS_TWOSIDEDSTENCILMODE);

	RSDefine(StencilFrontFunction, D3DRS_STENCILFUNC);

	RSDefine(StencilFrontSFail, D3DRS_STENCILFAIL);

	RSDefine(StencilFrontSPassZFail, D3DRS_STENCILZFAIL);

	RSDefine(StencilFrontAllPass, D3DRS_STENCILPASS);

	RSDefine(StencilBackFunction, D3DRS_CCW_STENCILFUNC);

	RSDefine(StencilBackSFail, D3DRS_CCW_STENCILFAIL);

	RSDefine(StencilBackSPassZFail, D3DRS_CCW_STENCILZFAIL);

	RSDefine(StencilBackAllPass, D3DRS_CCW_STENCILPASS);

	RSDefine(DepthTest, D3DRS_ZENABLE); // it depend , True because we use auto stencilMap when create Device.

	RSDefine(DepthWrite, D3DRS_ZWRITEENABLE);

	RSDefine(DepthFunction, D3DRS_ZFUNC);

	RSDefine(DepthBias, D3DRS_DEPTHBIAS);

	RSDefine(AlphaTest, D3DRS_ALPHATESTENABLE);

	RSDefine(AlphaFunction, D3DRS_ALPHAFUNC);

	RSDefine(AlphaReference, D3DRS_ALPHAREF);

	RSDefine(AlphaBlending, D3DRS_ALPHABLENDENABLE);

	RSDefine(SeperateAlphaBlending, D3DRS_SEPARATEALPHABLENDENABLE);

	RSDefine(BlendingOp, D3DRS_BLENDOP);

	RSDefine(SrcBlending, D3DRS_SRCBLEND);

	RSDefine(DstBlending, D3DRS_DESTBLEND);

	RSDefine(AlphaBlendingOp, D3DRS_BLENDOPALPHA);

	RSDefine(AlphaSrcBlending, D3DRS_SRCBLENDALPHA);

	RSDefine(AlphaDstBlending, D3DRS_DESTBLENDALPHA);

	TSSDefine(TextureColorOp, D3DTSS_COLOROP);

	TSSDefine(TextureColorArg1, D3DTSS_COLORARG1);

	TSSDefine(TextureColorArg2, D3DTSS_COLORARG2);

	TSSDefine(TextureAlphaOp, D3DTSS_ALPHAOP);

	TSSDefine(TextureAlphaArg1, D3DTSS_ALPHAARG1);

	TSSDefine(TextureAlphaArg2, D3DTSS_ALPHAARG2);

	SSDefine(SamplerMinFilter, D3DSAMP_MINFILTER);

	SSDefine(SamplerMagFilter, D3DSAMP_MAGFILTER);

	SSDefine(SamplerMipFilter, D3DSAMP_MIPFILTER);

	SSDefine(SamplerAddressU, D3DSAMP_ADDRESSU);

	SSDefine(SamplerAddressV, D3DSAMP_ADDRESSV);

	SSDefine(SamplerBorderColor, D3DSAMP_BORDERCOLOR);

protected:
	IDirect3DDevice9* m_pDevice;

	enum
	{
		RenderStateCount = D3DRS_BLENDOPALPHA + 1,
		TextureSlotCount = 8,
		TextureStageStateCount = D3DTSS_CONSTANT + 1,
		SamplerStateCount = D3DSAMP_DMAPOFFSET + 1,

	};
	std::vector<DWORD> m_RSValues;
	std::vector<DWORD> m_TSSValues[TextureSlotCount];
	std::vector<DWORD> m_SSValues[TextureSlotCount];

};

class FXStateManager : public ID3DXEffectStateManager, public StateManager
{
protected:
	RefCount m_refCount;
	WCHAR m_wszFrameStats[256];

public:
	FXStateManager(IDirect3DDevice9* pDevice);

	virtual LPCWSTR EndFrameStats();

	// methods inherited from ID3DXEffectStateManager
	STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv);

	STDMETHOD_(ULONG, AddRef)(THIS);

	STDMETHOD_(ULONG, Release)(THIS);

	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE d3dRenderState, DWORD dwValue);

	STDMETHOD(SetSamplerState)(THIS_ DWORD dwStage, D3DSAMPLERSTATETYPE d3dSamplerState, DWORD dwValue);

	STDMETHOD(SetTextureStageState)(THIS_ DWORD dwStage, D3DTEXTURESTAGESTATETYPE d3dTextureStageState, DWORD dwValue);

	STDMETHOD(SetTexture)(THIS_ DWORD dwStage, LPDIRECT3DBASETEXTURE9 pTexture);

	STDMETHOD(SetVertexShader)(THIS_ LPDIRECT3DVERTEXSHADER9 pShader);

	STDMETHOD(SetPixelShader)(THIS_ LPDIRECT3DPIXELSHADER9 pShader);

	STDMETHOD(SetFVF)(THIS_ DWORD dwFVF);

	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix);

	STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL9 *pMaterial);

	STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT9 *pLight);

	STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable);

	STDMETHOD(SetNPatchMode)(THIS_ FLOAT NumSegments);

	STDMETHOD(SetVertexShaderConstantF)(THIS_ unsigned int RegisterIndex, CONST FLOAT *pConstantData, unsigned int RegisterCount);

	STDMETHOD(SetVertexShaderConstantI)(THIS_ unsigned int RegisterIndex, CONST INT *pConstantData, unsigned int RegisterCount);

	STDMETHOD(SetVertexShaderConstantB)(THIS_ unsigned int RegisterIndex, CONST BOOL *pConstantData, unsigned int RegisterCount);

	STDMETHOD(SetPixelShaderConstantF)(THIS_ unsigned int RegisterIndex, CONST FLOAT *pConstantData, unsigned int RegisterCount);

	STDMETHOD(SetPixelShaderConstantI)(THIS_ unsigned int RegisterIndex, CONST INT *pConstantData, unsigned int RegisterCount);

	STDMETHOD(SetPixelShaderConstantB)(THIS_ unsigned int RegisterIndex, CONST BOOL *pConstantData, unsigned int RegisterCount);
};