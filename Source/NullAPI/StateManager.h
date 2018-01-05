#pragma once
#include <vector>
#include "../../RenderAPI/RenderAPI.h"

#include "RefCount.hpp"

#define RSDefine(name, rs) void Set##name(DWORD value) { SetRS(rs, value); }
#define TSSDefine(name, rs) void Set##name(unsigned int slot, DWORD value) { SetTSS(slot,rs, value); }
#define SSDefine(name, rs) void Set##name(unsigned int slot, DWORD value) { SetSS(slot,rs, value); }

class RenderStatistic;

class StateManager
{
public:

	StateManager(RenderStatistic& renderStatistic);

	~StateManager();

	bool SetRS(D3DRENDERSTATETYPE type, DWORD value);

	bool SetTSS(unsigned int slot, D3DTEXTURESTAGESTATETYPE type, DWORD value);

	bool SetSS(unsigned int slot, D3DSAMPLERSTATETYPE type, DWORD value);

	RSDefine(TextureFactor, D3DRS_TEXTUREFACTOR);

	void SetFillMode(RenderAPI::FillMode fillMode);

	void SetCullMode(RenderAPI::CullMode cukkMode);

	RSDefine(ScissorTest, D3DRS_SCISSORTESTENABLE);

	RSDefine(ColorWriteEnable, D3DRS_COLORWRITEENABLE);
	
	void SetStencilTest(bool enable);
	
	void SetStencilReference(unsigned int ref);
	
	void SetStencilReadMask(unsigned int mask);
	
	void SetStencilWriteMask(unsigned int mask);
	
	void SetStencilTwoFace(bool enable);
	
	void SetStencilFrontFunction(RenderAPI::CompareMethod func);
	
	void SetStencilFrontSFail(RenderAPI::StencilOp sop);
	
	void SetStencilFrontSPassZFail(RenderAPI::StencilOp sop);
	
	void SetStencilFrontAllPass(RenderAPI::StencilOp sop);
	
	void SetStencilBackFunction(RenderAPI::CompareMethod func);
	
	void SetStencilBackSFail(RenderAPI::StencilOp sop);
	
	void SetStencilBackSPassZFail(RenderAPI::StencilOp sop);
	
	void SetStencilBackAllPass(RenderAPI::StencilOp sop);
	
	void SetDepthTest(bool enable);
	
	RSDefine(DepthWrite, D3DRS_ZWRITEENABLE);
	
	void SetDepthFunction(RenderAPI::CompareMethod func);
	
	RSDefine(DepthBias, D3DRS_DEPTHBIAS);

	RSDefine(SlopScaleDepthBias, D3DRS_SLOPESCALEDEPTHBIAS);
	
	void SetAlphaTest(bool enable);
	
	void SetAlphaFunction(RenderAPI::CompareMethod func);

	void SetAlphaReference(unsigned char ref);

	void SetAlphaBlending(bool enable);
	
	void SetSeparateAlphaBlending(bool enable);

	void SetClipPlaneState(bool isEnable);

	RSDefine(Fog, D3DRS_FOGENABLE);

	RSDefine(FogTableMode, D3DRS_FOGTABLEMODE);

	RSDefine(FogStartDepth, D3DRS_FOGSTART);

	RSDefine(FogEndDepth, D3DRS_FOGEND);

	RSDefine(FogDensity, D3DRS_FOGDENSITY);

	RSDefine(FogColor, D3DRS_FOGCOLOR);
	
	void SetBlendingOp(RenderAPI::BlendOperator blendOp);

	void SetSrcBlending(RenderAPI::BlendFactor blendFactor);

	void SetDstBlending(RenderAPI::BlendFactor blendFactor);

	void SetAlphaBlendingOp(RenderAPI::BlendOperator blendOp);

	void SetAlphaSrcBlending(RenderAPI::BlendFactor blendFactor);

	void SetAlphaDstBlending(RenderAPI::BlendFactor blendFactor);

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

	SSDefine(SamplerAddressW, D3DSAMP_ADDRESSW);

	SSDefine(SamplerBorderColor, D3DSAMP_BORDERCOLOR);

	SSDefine(MaxAnisotropy, D3DSAMP_MAXANISOTROPY);

	bool GetClipplaneenable() const { return m_isClipplaneenable; }
	RenderAPI::BlendState GetBlendState() const { return m_blendState; }

	RenderAPI::BlendState GetAlphaSeparateBlendState() const { return m_alphaSeparateBlendState; }

	RenderAPI::AlphaTestingState GetAlphaTestingState() const { return m_alphaTestState; }

	RenderAPI::DepthTestingState GetDepthTestingState() const { return m_depthTestState; }

	RenderAPI::StencilTestingState GetStencilTestingState() const { return m_stencilTestState; }

	RenderAPI::FillMode GetFillMode() const { return m_fillMode; }

	RenderAPI::CullMode GetCullMode() const { return m_cullMode; }

	bool IsScissorTestEnable() const;

	void SetSupportMinAnisotropic(bool isSupport) { m_notSupportMinAnisotropic = !isSupport; }

	void SetSupportMagAnisotropic(bool isSupport) { m_notSupportMagAnisotropic = !isSupport; }


protected:

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

	bool m_isClipplaneenable;
	bool m_notSupportMinAnisotropic;
	bool m_notSupportMagAnisotropic;
	RenderAPI::BlendState m_blendState;
	RenderAPI::BlendState m_alphaSeparateBlendState;
	RenderAPI::AlphaTestingState m_alphaTestState;
	RenderAPI::DepthTestingState m_depthTestState;
	RenderAPI::StencilTestingState m_stencilTestState;
	RenderAPI::FillMode m_fillMode;
	RenderAPI::CullMode m_cullMode;

	RenderStatistic& m_renderStatistic;
};