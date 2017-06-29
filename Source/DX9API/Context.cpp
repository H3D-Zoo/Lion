#include "Context.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "APIContext.h"

namespace
{
	D3DPRIMITIVETYPE s_primitives[] =
	{
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_TRIANGLEFAN,
		D3DPT_LINELIST,
		D3DPT_LINESTRIP,
	};

	unsigned int s_cullMode[] =
	{
		D3DCULL_NONE,
		D3DCULL_CW,
		D3DCULL_CCW,
	};

	unsigned int s_fillMode[] =
	{
		D3DFILL_SOLID,
		D3DFILL_WIREFRAME,
		D3DFILL_POINT,
	};

	struct SamplerFilter
	{
		D3DTEXTUREFILTERTYPE min;
		D3DTEXTUREFILTERTYPE mag;
		D3DTEXTUREFILTERTYPE mip;
	};

	SamplerFilter s_samplerFilters[] =
	{
		{ D3DTEXF_POINT ,D3DTEXF_POINT , D3DTEXF_POINT },
		{ D3DTEXF_POINT ,D3DTEXF_POINT , D3DTEXF_LINEAR },
		{ D3DTEXF_POINT ,D3DTEXF_LINEAR , D3DTEXF_POINT },
		{ D3DTEXF_POINT ,D3DTEXF_LINEAR , D3DTEXF_LINEAR },
		{ D3DTEXF_LINEAR ,D3DTEXF_POINT , D3DTEXF_POINT },
		{ D3DTEXF_LINEAR ,D3DTEXF_POINT , D3DTEXF_LINEAR },
		{ D3DTEXF_LINEAR ,D3DTEXF_LINEAR  , D3DTEXF_POINT },
		{ D3DTEXF_LINEAR ,D3DTEXF_LINEAR  , D3DTEXF_LINEAR },
		{ D3DTEXF_POINT ,D3DTEXF_POINT , D3DTEXF_NONE },
		{ D3DTEXF_POINT ,D3DTEXF_LINEAR  , D3DTEXF_NONE },
		{ D3DTEXF_LINEAR ,D3DTEXF_POINT , D3DTEXF_NONE },
		{ D3DTEXF_LINEAR ,D3DTEXF_LINEAR  , D3DTEXF_NONE },
	};

	D3DTEXTUREADDRESS s_textureAddress[] =
	{
		D3DTADDRESS_WRAP,
		D3DTADDRESS_CLAMP,
		D3DTADDRESS_MIRROR,
		D3DTADDRESS_BORDER,
	};

	D3DTEXTUREOP s_texColorOps[] =
	{
		D3DTOP_DISABLE,
		D3DTOP_SELECTARG1,
		D3DTOP_SELECTARG2,
		D3DTOP_MODULATE,
		D3DTOP_MODULATE2X,
		D3DTOP_MODULATE4X,
		D3DTOP_ADD,
		D3DTOP_ADDSIGNED,
		D3DTOP_ADDSIGNED2X,
		D3DTOP_SUBTRACT,
		D3DTOP_ADDSMOOTH ,
		D3DTOP_DOTPRODUCT3,
		D3DTOP_MULTIPLYADD,
		D3DTOP_LERP,
	};

	unsigned int s_texBlendingArgs[] =
	{
		D3DTA_TEXTURE,
		D3DTA_CONSTANT,
		D3DTA_CURRENT,
		D3DTA_TFACTOR,
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

	unsigned int s_blendOps[] =
	{
		D3DBLENDOP_ADD,
		D3DBLENDOP_SUBTRACT,
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

	struct DeclFormat
	{
		D3DDECLTYPE Type;
		int Length;
	};

	DeclFormat s_declTypes[] =
	{
		{ D3DDECLTYPE_FLOAT1, sizeof(float) },
		{ D3DDECLTYPE_FLOAT2, sizeof(float) * 2},
		{ D3DDECLTYPE_FLOAT3, sizeof(float) * 3 },
		{ D3DDECLTYPE_FLOAT4, sizeof(float) * 4 },
		{ D3DDECLTYPE_D3DCOLOR, sizeof(unsigned char) * 4},
		{ D3DDECLTYPE_UBYTE4, sizeof(unsigned char) * 4 },
		{ D3DDECLTYPE_SHORT2, sizeof(short) * 2},
		{ D3DDECLTYPE_SHORT4, sizeof(short) * 4 },
		{ D3DDECLTYPE_UBYTE4N, sizeof(unsigned char) * 4},
		{ D3DDECLTYPE_SHORT2N, sizeof(short) * 2},
		{ D3DDECLTYPE_SHORT4N, sizeof(short) * 4},
		{ D3DDECLTYPE_USHORT2N, sizeof(unsigned short) * 2 },
		{ D3DDECLTYPE_USHORT4N, sizeof(unsigned short) * 4 },
	};

	D3DDECLUSAGE s_declUsages[] =
	{
		D3DDECLUSAGE_POSITION,
		D3DDECLUSAGE_COLOR,
		D3DDECLUSAGE_NORMAL,
		D3DDECLUSAGE_TEXCOORD,
	};
}

Context::Context(APIContext* pAPIContext, IDirect3DDevice9 * device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS)
	: m_pAPIContext(pAPIContext)
	, m_pDevice(device)
	, m_backBufferManager(device, defRT, defDS)
	, m_fxStateManager(device)
	, m_vertexDeclChanged(false)
	, m_pVertexDeclaraion(NULL)
{
	m_pAPIContext->pContext = this;
}

Context::~Context()
{
	m_pDevice->Release();
	m_pAPIContext->Release();
	m_pDevice = NULL;
	m_pAPIContext = NULL;
}

void Context::ClearRenderTarget(RenderAPI::RenderTarget* rt, unsigned int color)
{
	m_backBufferManager.SetTemporaryRT0(rt);
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 0, 0);
	m_backBufferManager.ResetRT0();
}

void Context::ClearDepthStencil(RenderAPI::DepthStencil* ds, float z, unsigned int stencil)
{
	m_backBufferManager.SetTemporaryDS(ds);
	m_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, z, stencil);
	m_backBufferManager.ResetDS();
}

void Context::SetViewport(const RenderAPI::Viewport& vp)
{
	D3DVIEWPORT9 d3dViewPort;
	d3dViewPort.X = vp.Left;
	d3dViewPort.Y = vp.Top;
	d3dViewPort.Width = vp.Width;
	d3dViewPort.Height = vp.Height;
	d3dViewPort.MinZ = vp.MinZ;
	d3dViewPort.MaxZ = vp.MaxZ;

	m_pDevice->SetViewport(&d3dViewPort);
}

void Context::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget)
{
	m_backBufferManager.SetRenderTarget(index, renderTarget);
}

void Context::SetDepthStencil(RenderAPI::DepthStencil* depthStencil)
{
	m_backBufferManager.SetDepthStencil(depthStencil);
}

void Context::SetVertexBuffers(unsigned int startSlot, RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount)
{
	for (unsigned int i = 0; i < bufferCount; i++)
	{
		IDirect3DVertexBuffer9* vertexBufferPtr = ((::VertexBuffer*)buffers[i].BufferPtr)->GetBufferPtr();
		m_pDevice->SetStreamSource(startSlot + i, vertexBufferPtr, buffers[i].Offset, buffers[i].Stride);
		SetVertexElements(startSlot + i, buffers[i].BufferPtr->GetElementPtr(), buffers[i].BufferPtr->GetElementCount());
	}
	m_vertexCount = buffers[0].BufferPtr->GetVertexCount();
}

void Context::SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset)
{
	IDirect3DIndexBuffer9* indexBufferPtr = ((::IndexBuffer*)buffer)->GetD3DIndexBuffer();
	m_pDevice->SetIndices(indexBufferPtr);
	m_indexBufferOffset = offset;
}

void Context::SetTextures(unsigned int startSlot, RenderAPI::Texture2D** textures, unsigned int resCount)
{
	for (unsigned int i = 0; i < resCount; i++)
	{
		IDirect3DTexture9* pTexture = ((::Texture2D*)textures[i])->GetD3DTexture();
		m_pDevice->SetTexture(startSlot + i, pTexture);
	}
}

void Context::SetBlendState(const RenderAPI::BlendState& state)
{
	if (state.IsEnable)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		if (state.IsAlphaSeperate)
		{
			m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);

			m_pDevice->SetRenderState(D3DRS_SRCBLEND, s_blendFactors[state.ColorSrc]);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, s_blendFactors[state.ColorDst]);
			m_pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, s_blendFactors[state.AlphaSrc]);
			m_pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, s_blendFactors[state.AlphaDst]);
		}
		else
		{
			m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_BLENDOP, s_blendOps[state.ColorOp]);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, s_blendFactors[state.ColorSrc]);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, s_blendFactors[state.ColorDst]);

		}
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

void Context::SetAlphaTestingState(const RenderAPI::AlphaTestingState& state)
{
	if (state.IsEnable)
	{
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, s_compareMethods[state.Function]);
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, state.Reference);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

void Context::SetDepthTestingState(const RenderAPI::DepthTestingState& state)
{
	if (state.IsEnable)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZFUNC, s_compareMethods[state.Function]);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		//m_pDevice->SetRenderState(D3DRS_ZFUNC, s_compareMethods[RenderAPI::COMPARE_Always]);
	}
}

void Context::SetStencilTestingState(const RenderAPI::StencilTestingState& state)
{
	if (state.IsEnable)
	{
		m_pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_STENCILREF, state.Reference);
		m_pDevice->SetRenderState(D3DRS_STENCILMASK, state.TestMask);
		m_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, state.WriteMask);
		if (state.TwoSide)
		{
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, s_compareMethods[state.FrontSide.Function]);
			m_pDevice->SetRenderState(D3DRS_STENCILFAIL, s_stencilOps[state.FrontSide.SFail]);
			m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, s_stencilOps[state.FrontSide.SPassZFail]);
			m_pDevice->SetRenderState(D3DRS_STENCILPASS, s_stencilOps[state.FrontSide.AllPass]);

			m_pDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, s_compareMethods[state.BackSide.Function]);
			m_pDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, s_stencilOps[state.BackSide.SFail]);
			m_pDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, s_stencilOps[state.BackSide.SPassZFail]);
			m_pDevice->SetRenderState(D3DRS_CCW_STENCILPASS, s_stencilOps[state.BackSide.AllPass]);
		}
		else
		{
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, TRUE);
			m_pDevice->SetRenderState(D3DRS_STENCILFUNC, s_compareMethods[state.FrontSide.Function]);
			m_pDevice->SetRenderState(D3DRS_STENCILFAIL, s_stencilOps[state.FrontSide.SFail]);
			m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, s_stencilOps[state.FrontSide.SPassZFail]);
			m_pDevice->SetRenderState(D3DRS_STENCILPASS, s_stencilOps[state.FrontSide.AllPass]);
		}
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
}

void Context::SetDepthWriting(bool enable)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, enable ? TRUE : FALSE);
}

void Context::SetTextureBlendingState(unsigned int startSlot, const RenderAPI::TextureBlendingState* states, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		const RenderAPI::TextureBlendingState& state = states[i];
		if (state.ColorOp == RenderAPI::TEXOP_Disable)
		{
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		}
		else
		{
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_COLOROP, s_texColorOps[state.ColorOp]);
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_ALPHAARG0, s_texBlendingArgs[state.AlphaArg0]);
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_ALPHAARG1, s_texBlendingArgs[state.AlphaArg1]);
		}

		if (state.AlphaOp == RenderAPI::TEXOP_Disable)
		{
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		}
		else
		{
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_ALPHAOP, s_texColorOps[state.AlphaOp]);
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_ALPHAARG0, s_texBlendingArgs[state.AlphaArg0]);
			m_pDevice->SetTextureStageState(startSlot + i, D3DTSS_ALPHAARG1, s_texBlendingArgs[state.AlphaArg1]);
		}
	}
}

void Context::SetTextureSampler(unsigned int startSlot, const RenderAPI::TextureSampler* samplers, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		const RenderAPI::TextureSampler& sampler = samplers[i];
		SamplerFilter& filter = s_samplerFilters[sampler.Filter];
		m_pDevice->SetSamplerState(startSlot + i, D3DSAMP_MINFILTER, filter.min);
		m_pDevice->SetSamplerState(startSlot + i, D3DSAMP_MAGFILTER, filter.mag);
		m_pDevice->SetSamplerState(startSlot + i, D3DSAMP_MIPFILTER, filter.mip);
		m_pDevice->SetSamplerState(startSlot + i, D3DSAMP_ADDRESSU, s_textureAddress[sampler.AddressU]);
		m_pDevice->SetSamplerState(startSlot + i, D3DSAMP_ADDRESSV, s_textureAddress[sampler.AddressV]);
		if (sampler.AddressV == RenderAPI::TEX_ADDRESS_Border || sampler.AddressU == RenderAPI::TEX_ADDRESS_Border)
		{
			m_pDevice->SetSamplerState(startSlot + i, D3DSAMP_BORDERCOLOR, sampler.BorderColor);
		};
	}
}

void Context::SetScissorState(const RenderAPI::ScissorState& state)
{
	if (state.IsEnable)
	{
		RECT scissorRect;
		scissorRect.left = state.Left;
		scissorRect.right = state.Right;
		scissorRect.top = state.Top;
		scissorRect.bottom = state.Bottom;
		m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		m_pDevice->SetScissorRect(&scissorRect);
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	}
}

void Context::SetFillMode(RenderAPI::FillMode mode)
{
	m_pDevice->SetRenderState(D3DRS_FILLMODE, s_fillMode[mode]);
}

void Context::SetCullMode(RenderAPI::CullMode mode)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, s_cullMode[mode]);
}

void Context::SetDepthBias(float bias)
{
	m_pDevice->SetRenderState(D3DRS_DEPTHBIAS, (DWORD)bias);
}

void Context::SetTextureFactor(unsigned int factor)
{
	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, factor);
}

bool Context::BeginScene()
{
	return S_OK == m_pDevice->BeginScene();
}

void Context::EndScene()
{
	m_pDevice->EndScene();
}

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount)
{
	RebuildDecalration();
	m_pDevice->DrawPrimitive(s_primitives[primitive], startIndex, primitiveCount);
}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount)
{
	RebuildDecalration();
	m_pDevice->DrawIndexedPrimitive(s_primitives[primitive], baseVertex, 0, m_vertexCount, startIndex, primitiveCount);
}

RenderAPI::DeviceState Context::CheckDeviceLost()
{
	HRESULT hr = m_pDevice->TestCooperativeLevel();
	if (hr == S_OK)
	{
		return RenderAPI::DEVICE_OK;
	}
	else if (hr == D3DERR_DEVICELOST)
	{
		return RenderAPI::DEVICE_Lost;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		return RenderAPI::DEVICE_WaitReset;
	}
	else
	{
		return RenderAPI::DEVICE_Error;
	}
}

bool Context::ResetDevice()
{
	return S_OK == m_pDevice->Reset(&(m_pAPIContext->CreationParam));
}

void Context::Release()
{
	delete this;
}


ID3DXEffectStateManager* Context::GetStateManager()
{
	return &m_fxStateManager;
}

void Context::SetVertexElements(int index, const RenderAPI::VertexElement * s, int count)
{
	if ((int)m_vertexDeclCache.size() <= index)
	{
		m_vertexDeclCache.resize(index + 1);
	}

	VertexDecl& decl = m_vertexDeclCache[index];
	if (decl.Set(s, count) && !m_vertexDeclChanged)
	{
		m_vertexDeclChanged = true;
	}
}

bool operator<(const D3DVERTEXELEMENT9& left, const D3DVERTEXELEMENT9& right)
{
	if (left.Stream < right.Stream)
		return true;

	if (left.Offset < right.Offset)
		return true;

	if (left.Offset < right.Offset)
		return true;

	if (left.Type < right.Type)
		return true;

	if (left.Method < right.Method)
		return true;

	if (left.Usage < right.Usage)
		return true;

	if (left.UsageIndex < right.UsageIndex)
		return true;

	return false;
}

void Context::RebuildDecalration()
{
	if (m_vertexDeclChanged)
	{
		m_d3dDeclaration.clear();
		std::vector<VertexDecl>::iterator itCur = m_vertexDeclCache.begin();
		std::vector<VertexDecl>::iterator itEnd = m_vertexDeclCache.end();
		int offset = 0;
		for (int i = 0; itCur != itEnd; ++itCur, ++i)
		{
			D3DVERTEXELEMENT9 element;
			element.Stream = i;

			VertexDecl& decl = *itCur;
			std::vector<RenderAPI::VertexElement>::iterator itCurS = decl.Elements.begin();
			std::vector<RenderAPI::VertexElement>::iterator itEndS = decl.Elements.end();
			for (; itCurS != itEndS; ++itCurS)
			{

				element.Offset = (itCurS->AlignOffset == 0xFFFFFFFF) ? offset : itCurS->AlignOffset;
				DeclFormat& fmt = s_declTypes[itCurS->Format];
				element.Type = fmt.Type;
				offset += fmt.Length;
				element.Method = D3DDECLMETHOD_DEFAULT;
				element.Usage = s_declUsages[itCurS->SemanticName];
				element.UsageIndex = itCurS->SemanticIndex;

				m_d3dDeclaration.push_back(element);
			}
		}

		VertexDecalrationPool::iterator findIter = m_vertexDeclarationPool.find(m_d3dDeclaration);
		if (findIter != m_vertexDeclarationPool.end())
		{
			m_pVertexDeclaraion = findIter->second;
			m_pDevice->SetVertexDeclaration(m_pVertexDeclaraion);
		}

		m_vertexDeclChanged = false;
	}
}

BackBufferManager::BackBufferManager(IDirect3DDevice9 * device, RenderAPI::RenderTarget * defRT, RenderAPI::DepthStencil * defDS)
	: m_pDevice(device)
	, m_pCurrentRTs(1)
	, m_pLastRT0(NULL)
	, m_pLastDS(NULL)
{
	m_pDevice->AddRef();
	m_pCurrentRTs[0] = ((::RenderTarget*)defRT)->GetD3DSurface();
	m_pCurrentDS = ((::DepthStencil*)defDS)->GetD3DSurface();
}

BackBufferManager::~BackBufferManager()
{
	m_pDevice->Release();
}

void BackBufferManager::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget * rt)
{
	IDirect3DSurface9* rtSurface = ((::RenderTarget*)rt)->GetD3DSurface();

	if (index >= m_pCurrentRTs.size())
	{
		m_pCurrentRTs.resize(index + 1);
		m_pDevice->SetRenderTarget(index, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
	else if (rtSurface != m_pCurrentRTs[index])
	{
		m_pDevice->SetRenderTarget(0, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
}

void BackBufferManager::SetDepthStencil(RenderAPI::DepthStencil * rt)
{
	IDirect3DSurface9* dsSurface = ((::DepthStencil*)rt)->GetD3DSurface();
	if (dsSurface != m_pCurrentDS)
	{
		m_pDevice->SetDepthStencilSurface(dsSurface);
		m_pCurrentDS = dsSurface;
	}
}

void BackBufferManager::SetTemporaryRT0(RenderAPI::RenderTarget * rt)
{
	IDirect3DSurface9* rtSurface = ((::RenderTarget*)rt)->GetD3DSurface();
	if (m_pCurrentRTs[0] != rtSurface)
	{
		if (m_pLastRT0 == NULL)
		{
			m_pLastRT0 = m_pCurrentRTs[0];
		}

		m_pCurrentRTs[0] = rtSurface;
		m_pDevice->SetRenderTarget(0, rtSurface);
	}
}

void BackBufferManager::SetTemporaryDS(RenderAPI::DepthStencil * rt)
{
	IDirect3DSurface9* dsSurface = ((::DepthStencil*)rt)->GetD3DSurface();
	if (dsSurface != m_pCurrentDS)
	{
		if (m_pLastDS == NULL)
		{
			m_pLastDS = m_pCurrentDS;
		}

		m_pDevice->SetDepthStencilSurface(dsSurface);
		m_pCurrentDS = dsSurface;
	}
}

void BackBufferManager::ResetRT0()
{
	if (m_pLastRT0 != NULL)
	{
		m_pCurrentRTs[0] = m_pLastRT0;
		m_pDevice->SetRenderTarget(0, m_pLastRT0);
	}
}

void BackBufferManager::ResetDS()
{
	if (m_pLastDS != NULL)
	{
		m_pCurrentDS = m_pLastDS;
		m_pDevice->SetDepthStencilSurface(m_pLastDS);
	}
}

bool operator != (const RenderAPI::VertexElement& left, const RenderAPI::VertexElement& right)
{
	return  (left.SemanticName != right.SemanticName ||
		left.SemanticIndex != right.SemanticIndex ||
		left.AlignOffset != right.AlignOffset ||
		left.Format != right.Format);
}

bool Context::VertexDecl::Set(const RenderAPI::VertexElement * s, int count)
{
	if ((int)Elements.size() <= count)
	{
		Elements.resize(count);
		memcpy(&(Elements[0]), s, sizeof(RenderAPI::VertexElement) * count);
		Count = count;
		return true;
	}
	else
	{
		bool same = false;
		if (Count == count)
		{
			same = true;
			for (int i = 0; i < Count; i++)
			{
				if (Elements[i] != s[i])
				{
					same = false;
					break;
				}
			}
		}

		if (!same)
		{
			memcpy(&(Elements[0]), s, sizeof(RenderAPI::VertexElement) * count);
			Count = count;
			return true;
		}
		else
		{
			return false;
		}
	}
}
