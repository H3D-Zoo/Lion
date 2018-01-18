#include "Context.h"
#include "APIInstance.h"
#include "Device.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "EnumMapping.h"
#include "AutoPtr.hpp"
#include "Texture2D.h"
#include "TextureCube.h"

struct DDrawX
{
	DDrawX() : m_pDD7(NULL)
	{
		IDirectDraw2* pDD2;
		CoInitialize(NULL);
		HRESULT ddrval = CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_ALL, IID_IDirectDraw2, (void**)&pDD2);
		if (SUCCEEDED(ddrval))
		{
			ddrval = IDirectDraw2_Initialize(pDD2, NULL);
			pDD2->QueryInterface(IID_IDirectDraw7, (LPVOID*)&m_pDD7);
		}
		pDD2->Release();
	}

	~DDrawX()
	{
		if (m_pDD7 != NULL)
		{
			m_pDD7->Release();
		}
	}

	unsigned int GetAvailableVideoMemory()
	{
		if (m_pDD7 != NULL)
		{
			DDSCAPS2 ddsCaps2;

			/* û���õĵ��ã����岻����ע�͵�
			// video memory
			ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
			ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY;
			DWORD dwTotal = 0;
			DWORD dwFree = 0;
			m_pDD7->GetAvailableVidMem(&ddsCaps2, &dwTotal, &dwFree);
			*/

			// video memory (local)
			ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
			ddsCaps2.dwCaps = DDSCAPS_LOCALVIDMEM;
			DWORD dwTotalLocal = 0;
			DWORD dwFreeLocal = 0;
			m_pDD7->GetAvailableVidMem(&ddsCaps2, &dwTotalLocal, &dwFreeLocal);

			/* û���õĵ��ã����岻����ע�͵�
			// video memory (non-local)
			ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));
			ddsCaps2.dwCaps = DDSCAPS_NONLOCALVIDMEM;
			DWORD dwTotalNonLocal = 0;
			DWORD dwFreeNonLocal = 0;
			m_pDD7->GetAvailableVidMem(&ddsCaps2, &dwTotalNonLocal, &dwFreeNonLocal);
			*/

			return dwTotalLocal;
		}
		else
		{
			return 0;
		}
	}

private:
	//by sssa2000 2008.03.11
	IDirectDraw7 * m_pDD7;
};

namespace
{
	D3DPRIMITIVETYPE s_primitives[] =
	{
		D3DPT_TRIANGLELIST,
		D3DPT_TRIANGLESTRIP,
		D3DPT_TRIANGLEFAN,
		D3DPT_LINELIST,
		D3DPT_LINESTRIP,
		D3DPT_POINTLIST,
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
}
Context::Context(APIInstance* pAPI, IDirect3DDevice9 * device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS)
	: m_pAPI(pAPI)
	, m_pDevice(device)
	, m_backBufferManager(device, defRT, defDS, pAPI->GetRenderStatistic(), *pAPI)
	, m_renderStateManager(device, pAPI->GetRenderStatistic())
	, m_nNXCacheFVF(0)
	, m_pNXCacheVertexShader(NULL)
	, m_pNXCachePixelShader(NULL)
	, m_pNXCacheTexture(NULL)
{
	m_pAPI->AddRef();

	if (m_pAPI->IsSupportD3D9EX())
	{
		m_pDeviceEx = (IDirect3DDevice9Ex*)m_pDevice;
	}

	RECT scissorRect;
	m_pDevice->GetScissorRect(&scissorRect);
	m_scissorState.Left = scissorRect.left;
	m_scissorState.Right = scissorRect.right;
	m_scissorState.Top = scissorRect.top;
	m_scissorState.Bottom = scissorRect.bottom;

	D3DCAPS9 d3dcaps;
	m_pDevice->GetDeviceCaps(&d3dcaps);
	bool minAnisotropic = (d3dcaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) != 0;
	bool magAnisotropic = (d3dcaps.TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) != 0;
	m_renderStateManager.SetSupportMinAnisotropic(minAnisotropic);
	m_renderStateManager.SetSupportMagAnisotropic(magAnisotropic);
	m_backBufferManager.SetMaxTextureStage(d3dcaps.MaxTextureBlendStages);

	InitDeviceCaps(d3dcaps);
}

Context::~Context()
{
	m_pDevice->Release();
	m_pAPI->Release();
	m_pDeviceEx = NULL;
	m_pDevice = NULL;
	m_pAPI = NULL;
}

const RenderAPI::DeviceCaps& Context::GetDeviceCaps() const
{
	return m_deviceCaps;
}

unsigned int Context::GetAvailableTextureMemory() const
{
	return m_pDevice->GetAvailableTextureMem();
}

void Context::ClearRenderTarget(unsigned int color)
{
	LOG_FUNCTION_V(*m_pAPI, "color=%X", color);

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 0, 0);
}

void Context::ClearDepthBuffer(float z)
{
	LOG_FUNCTION_V(*m_pAPI, "z=%f", z);

	m_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, z, 0);
}

void Context::ClearStencilBuffer(unsigned int stencil)
{
	LOG_FUNCTION_V(*m_pAPI, "stencil=%X", stencil);

	m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, stencil);
}

void Context::SetViewport(const RenderAPI::Viewport& vp)
{
	LOG_FUNCTION_D(*m_pAPI, "left=%d, top=%d, width=%d, height=%d, minz=%f, maxz=%f",
		vp.Left, vp.Top, vp.Width, vp.Height, vp.MinZ, vp.MaxZ);

	D3DVIEWPORT9 d3dViewPort;
	d3dViewPort.X = vp.Left;
	d3dViewPort.Y = vp.Top;
	d3dViewPort.Width = vp.Width;
	d3dViewPort.Height = vp.Height;
	d3dViewPort.MinZ = vp.MinZ;
	d3dViewPort.MaxZ = vp.MaxZ;

	m_pDevice->SetViewport(&d3dViewPort);
}

RenderAPI::Viewport Context::GetViewport()
{
	D3DVIEWPORT9 vp;
	m_pDevice->GetViewport(&vp);

	RenderAPI::Viewport viewport;
	viewport.Left = vp.X;
	viewport.Top = vp.Y;
	viewport.Width = vp.Width;
	viewport.Height = vp.Height;
	viewport.MinZ = vp.MinZ;
	viewport.MaxZ = vp.MaxZ;
	return viewport;
}

void Context::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget)
{
	m_backBufferManager.SetRenderTarget(index, renderTarget);
}

void Context::SetDepthStencil(RenderAPI::DepthStencil* depthStencil)
{
	m_backBufferManager.SetDepthStencil(depthStencil);
}

void Context::SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount)
{
	LOG_FUNCTION_CALL(*m_pAPI, RenderAPI::LOG_Verbose);

	while (m_useStreamFrequency.size() < bufferCount)
	{
		m_useStreamFrequency.push_back(false);
	}

	m_vertexBufferCount = 0;
	for (unsigned int i = 0; i < bufferCount; i++)
	{
		if (buffers[i].BufferPtr != NULL)
		{
			::VertexBuffer* pVertexBuffer = (::VertexBuffer*)buffers[i].BufferPtr;
			IDirect3DVertexBuffer9* vertexBufferPtr = pVertexBuffer->GetBufferPtr();


			if (m_vertexBufferCount == 0 || m_vertexBufferCount > pVertexBuffer->GetVertexCount())
			{
				m_vertexBufferCount = pVertexBuffer->GetVertexCount();
			}
			HRESULT hr = m_pDevice->SetStreamSource(i, vertexBufferPtr, buffers[i].Offset, buffers[i].BufferPtr->GetVertexStride());

			bool useFreq = buffers->FrequencyMethod != RenderAPI::FREQUENCY_Normal;
			if (useFreq)
			{
				unsigned int freqParam = (buffers->FrequencyMethod == RenderAPI::FREQUENCY_Instanced ? D3DSTREAMSOURCE_INSTANCEDATA : D3DSTREAMSOURCE_INDEXEDDATA)
					| buffers[i].OptionalFrequencyInterval;

				m_pDevice->SetStreamSourceFreq(i, freqParam);
				m_useStreamFrequency[i] = true;
			}
			else if (m_useStreamFrequency[i])
			{
				m_pDevice->SetStreamSourceFreq(i, 1);
				m_useStreamFrequency[i] = false;
			}

			//ͳ����Ϣ
			m_pAPI->GetRenderStatistic().OnSetSourceStream(pVertexBuffer);
		}
	}
}

void Context::SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset)
{
	LOG_FUNCTION_V(*m_pAPI, "index buffer=%X, offset=%d", buffer, offset);

	if (buffer != NULL)
	{
		::IndexBuffer* pIndexBuffer = (::IndexBuffer*)buffer;
		m_pDevice->SetIndices(pIndexBuffer->GetD3DIndexBuffer());
		m_indexBufferOffset = offset;

		m_pAPI->GetRenderStatistic().OnSetIndexBuffer(pIndexBuffer);
	}
}

void Context::SetVertexDeclaration(RenderAPI::VertexDeclaration * decl)
{
	LOG_FUNCTION_V(*m_pAPI, "vertex declaration=%X", decl);
	if (decl != NULL)
	{
		::VertexDeclaration* pDeclaration = (::VertexDeclaration*)decl;
		m_pDevice->SetFVF(pDeclaration->GetCustomFVF());
	}
	else
	{
		m_pDevice->SetFVF(0);
	}

	m_pAPI->GetRenderStatistic().OnSetVertexDeclaration();
}

void Context::SetTexture(unsigned int slot, RenderAPI::Texture* texture)
{
	LOG_FUNCTION_V(*m_pAPI, "slot=%d, texture=%X", slot, texture);

	IDirect3DBaseTexture9* pTexture = NULL;
	if (texture != NULL)
	{
		if (texture->IsCubemap())
			pTexture = ((::TextureCube*)texture)->GetD3DTexture();
		else
			pTexture = ((::Texture2D*)texture)->GetD3DTexture();
	}
	m_pDevice->SetTexture(slot, pTexture);

	m_pAPI->GetRenderStatistic().OnSetTexture(texture);
}

void Context::SetBlendState(const RenderAPI::BlendState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d, op=%d, src=%d, dst=%d",
		state.IsEnable, state.BlendOp, state.SrcBlend, state.DstBlend);

	if (state.IsEnable)
	{
		m_renderStateManager.SetAlphaBlending(TRUE);

		m_renderStateManager.SetBlendingOp(state.BlendOp);
		m_renderStateManager.SetSrcBlending(state.SrcBlend);
		m_renderStateManager.SetDstBlending(state.DstBlend);
	}
	else
	{
		m_renderStateManager.SetAlphaBlending(FALSE);
	}
}

void Context::SetAlphaSeparateBlendState(const RenderAPI::BlendState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d, op=%d, src=%d, dst=%d",
		state.IsEnable, state.BlendOp, state.SrcBlend, state.DstBlend);

	if (state.IsEnable)
	{
		m_renderStateManager.SetSeparateAlphaBlending(TRUE);
		m_renderStateManager.SetAlphaBlendingOp(state.BlendOp);
		m_renderStateManager.SetAlphaSrcBlending(state.SrcBlend);
		m_renderStateManager.SetAlphaDstBlending(state.DstBlend);
	}
	else
	{
		m_renderStateManager.SetSeparateAlphaBlending(FALSE);
	}
}

RenderAPI::BlendState Context::GetBlendState() const
{
	return m_renderStateManager.GetBlendState();
}

RenderAPI::BlendState Context::GetAlphaSeparateBlendState() const
{
	return m_renderStateManager.GetAlphaSeparateBlendState();
}

void Context::SetAlphaTestingState(const RenderAPI::AlphaTestingState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d, function=%d, ref=%X",
		state.IsEnable, state.Function, state.Reference);

	if (state.IsEnable)
	{
		m_renderStateManager.SetAlphaTest(TRUE);
		m_renderStateManager.SetAlphaFunction(state.Function);
		m_renderStateManager.SetAlphaReference(state.Reference);
	}
	else
	{
		m_renderStateManager.SetAlphaTest(FALSE);
	}
}

RenderAPI::AlphaTestingState Context::GetAlphaTestingState() const
{
	return m_renderStateManager.GetAlphaTestingState();
}

void Context::SetDepthTestingState(const RenderAPI::DepthTestingState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d, function=%d", state.IsEnable, state.Function);

	if (state.IsEnable)
	{
		m_renderStateManager.SetDepthTest(D3DZB_TRUE);
		m_renderStateManager.SetDepthFunction(state.Function);
	}
	else
	{
		m_renderStateManager.SetDepthTest(D3DZB_FALSE);
		//m_renderStateManager.SetDepthFunction(s_compareMethods[RenderAPI::COMPARE_Always]);
	}
}

RenderAPI::DepthTestingState Context::GetDepthTestingState() const
{
	return m_renderStateManager.GetDepthTestingState();
}
void Context::SetClipPlaneState(bool enable)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d", enable);

	m_renderStateManager.SetClipPlaneState(enable);
}

bool Context::GetClipPlaneState() const
{
	return m_renderStateManager.GetClipPlaneEnable();
}

void Context::SetStencilTestingState(const RenderAPI::StencilTestingState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d, two side=%d, test mask=%X, write mask=%X, ref=%X,"
		"front function=%d, front sfail=%d, front zfail=%d, back pass=%d"
		"back function=%d, back sfail=%d, back zfail=%d, back pass=%d",
		state.IsEnable, state.TwoSide, state.TestMask, state.WriteMask, state.Reference,
		state.FrontSide.Function, state.FrontSide.SFail, state.FrontSide.SPassZFail, state.FrontSide.AllPass,
		state.BackSide.Function, state.BackSide.SFail, state.BackSide.SPassZFail, state.BackSide.AllPass);

	if (state.IsEnable)
	{
		m_renderStateManager.SetStencilTest(TRUE);
		m_renderStateManager.SetStencilReference(state.Reference);
		m_renderStateManager.SetStencilReadMask(state.TestMask);
		m_renderStateManager.SetStencilWriteMask(state.WriteMask);
		if (state.TwoSide)
		{
			m_renderStateManager.SetStencilTwoFace(TRUE);
			m_renderStateManager.SetStencilBackFunction(state.BackSide.Function);
			m_renderStateManager.SetStencilBackSFail(state.BackSide.SFail);
			m_renderStateManager.SetStencilBackSPassZFail(state.BackSide.SPassZFail);
			m_renderStateManager.SetStencilBackAllPass(state.BackSide.AllPass);
		}
		else
		{
			m_renderStateManager.SetStencilTwoFace(FALSE);
		}

		m_renderStateManager.SetStencilFrontFunction(state.FrontSide.Function);
		m_renderStateManager.SetStencilFrontSFail(state.FrontSide.SFail);
		m_renderStateManager.SetStencilFrontSPassZFail(state.FrontSide.SPassZFail);
		m_renderStateManager.SetStencilFrontAllPass(state.FrontSide.AllPass);
	}
	else
	{
		m_renderStateManager.SetStencilTest(FALSE);
	}
}

RenderAPI::StencilTestingState Context::GetStencilTestingState() const
{
	return m_renderStateManager.GetStencilTestingState();
}

void Context::SetDepthWriting(bool enable)
{
	LOG_FUNCTION_V(*m_pAPI, "enable=%d", enable);

	m_renderStateManager.SetDepthWrite(enable ? TRUE : FALSE);
}

void Context::SetTextureColorBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "slot=%d, op=%d, arg0=%d, arg1=%d",
		slot, state.BlendOp, state.Argument0, state.Argument1);

	if (state.BlendOp == RenderAPI::TEXOP_Disable)
	{
		m_renderStateManager.SetTextureColorOp(slot, D3DTOP_DISABLE);
	}
	else
	{
		m_renderStateManager.SetTextureColorOp(slot, s_texColorOps[state.BlendOp]);
		m_renderStateManager.SetTextureColorArg1(slot, s_texBlendingArgs[state.Argument0]);
		m_renderStateManager.SetTextureColorArg2(slot, s_texBlendingArgs[state.Argument1]);
	}
}

void Context::SetTextureAlphaBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state)
{
	LOG_FUNCTION_V(*m_pAPI, "slot=%d, op=%d, arg0=%d, arg1=%d",
		slot, state.BlendOp, state.Argument0, state.Argument1);

	if (state.BlendOp == RenderAPI::TEXOP_Disable)
	{
		m_renderStateManager.SetTextureColorOp(slot, D3DTOP_DISABLE);
	}
	else
	{
		m_renderStateManager.SetTextureAlphaOp(slot, s_texColorOps[state.BlendOp]);
		m_renderStateManager.SetTextureAlphaArg1(slot, s_texBlendingArgs[state.Argument0]);
		m_renderStateManager.SetTextureAlphaArg2(slot, s_texBlendingArgs[state.Argument1]);
	}
}

void Context::SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler)
{
	LOG_FUNCTION_V(*m_pAPI, "slot=%d, address u=%d, address v=%d, address w=%d",
		"min filter=%d, mag filter=%d, mip filter=%d,"
		"border color=%X, optional anisotrophic=%d",
		slot, sampler.AddressU, sampler.AddressV, sampler.AddressW, 
		sampler.MinFilter, sampler.MagFilter, sampler.MipFilter,
		sampler.BorderColor, sampler.OptionalAnisotropicFilter);

	m_renderStateManager.SetSamplerMinFilter(slot, s_d3dSamplerFilter[sampler.MinFilter]);
	m_renderStateManager.SetSamplerMagFilter(slot, s_d3dSamplerFilter[sampler.MagFilter]);
	m_renderStateManager.SetSamplerMipFilter(slot, s_d3dSamplerFilter[sampler.MipFilter]);
	m_renderStateManager.SetSamplerAddressU(slot, s_textureAddress[sampler.AddressU]);
	m_renderStateManager.SetSamplerAddressV(slot, s_textureAddress[sampler.AddressV]);
	m_renderStateManager.SetSamplerAddressW(slot, s_textureAddress[sampler.AddressW]);
	if (sampler.AddressU == RenderAPI::TEX_ADDRESS_Border ||
		sampler.AddressV == RenderAPI::TEX_ADDRESS_Border ||
		sampler.AddressW == RenderAPI::TEX_ADDRESS_Border)
	{
		m_renderStateManager.SetSamplerBorderColor(slot, sampler.BorderColor);
	};

	if (sampler.MinFilter == RenderAPI::FILTER_Anisotropic ||
		sampler.MagFilter == RenderAPI::FILTER_Anisotropic)
	{
		m_renderStateManager.SetMaxAnisotropy(slot, s_sampleTypes[sampler.OptionalAnisotropicFilter]);
	}
}

void Context::SetScissorState(const RenderAPI::ScissorState& state)
{
	LOG_FUNCTION_D(*m_pAPI, "enable=%d, l=%d, r=%d, t=%d, b=%d",
		state.IsEnable, state.Left, state.Right, state.Top, state.Bottom);

	if (state.IsEnable)
	{
		m_renderStateManager.SetScissorTest(TRUE);

		RECT scissorRect;
		scissorRect.left = state.Left;
		scissorRect.right = state.Right;
		scissorRect.top = state.Top;
		scissorRect.bottom = state.Bottom;
		m_scissorState = state;
		m_pDevice->SetScissorRect(&scissorRect);
	}
	else
	{
		m_renderStateManager.SetScissorTest(FALSE);
	}
}

RenderAPI::ScissorState Context::GetScissorState() const
{
	m_scissorState.IsEnable = m_renderStateManager.IsScissorTestEnable();
	return m_scissorState;
}

void Context::SetColorWriteMask(bool r, bool g, bool b, bool a)
{
	LOG_FUNCTION_V(*m_pAPI, "r=%d, g=%d, b=%d, a=%d", r, g, b, a);

	unsigned int mask = 0;
	if (r)
		mask |= D3DCOLORWRITEENABLE_RED;
	if (g)
		mask |= D3DCOLORWRITEENABLE_GREEN;
	if (b)
		mask |= D3DCOLORWRITEENABLE_BLUE;
	if (a)
		mask |= D3DCOLORWRITEENABLE_ALPHA;
	m_renderStateManager.SetColorWriteEnable(mask);
}

void Context::SetFillMode(RenderAPI::FillMode mode)
{
	LOG_FUNCTION_V(*m_pAPI, "mode=%d", mode);

	m_renderStateManager.SetFillMode(mode);
}

RenderAPI::FillMode Context::GetFillMode() const
{
	return m_renderStateManager.GetFillMode();
}

void Context::SetCullMode(RenderAPI::CullMode mode)
{
	LOG_FUNCTION_V(*m_pAPI, "mode=%d", mode);

	m_renderStateManager.SetCullMode(mode);
}

RenderAPI::CullMode Context::GetCullMode() const
{
	return m_renderStateManager.GetCullMode();
}

void Context::SetDepthBias(float bias)
{
	LOG_FUNCTION_V(*m_pAPI, "bias=%f", bias);

	m_renderStateManager.SetDepthBias((DWORD)bias);
}

void Context::SetSlopScaleDepthBias(float bias)
{
	LOG_FUNCTION_V(*m_pAPI, "bias=%f", bias);

	m_renderStateManager.SetSlopScaleDepthBias((DWORD)bias);
}

void Context::SetTextureFactor(unsigned int factor)
{
	LOG_FUNCTION_V(*m_pAPI, "factor=%d", factor);

	m_renderStateManager.SetTextureFactor(factor);
}

bool Context::BeginScene()
{
	LOG_FUNCTION_CALL(*m_pAPI, RenderAPI::LOG_Debug);
	return S_OK == m_pDevice->BeginScene();
}

void Context::EndScene()
{
	LOG_FUNCTION_CALL(*m_pAPI, RenderAPI::LOG_Debug);

	m_pDevice->EndScene();
}

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startVertex, unsigned int primitiveCount)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, start=%d, count=%d", primitive, startVertex, primitiveCount);

	m_pDevice->DrawPrimitive(s_primitives[primitive], startVertex, primitiveCount);

	m_pAPI->GetRenderStatistic().OnDraw(primitive, primitiveCount);
}

void Context::DrawWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int primitiveCount, const void* pVertexData, unsigned int vertexStride)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, count=%d, stride=%d, data=%X", primitive, primitiveCount, vertexStride, pVertexData);

	m_pDevice->DrawPrimitiveUP(s_primitives[primitive], primitiveCount, pVertexData, vertexStride);

	m_pAPI->GetRenderStatistic().OnDrawUp(primitive, primitiveCount);
}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, base index=%d, vertex count=%d, saved vertex count=%d, start index=%d, primitive count=%d",
		primitive, baseVertex, vertexCount, m_vertexBufferCount, startIndex, primitiveCount);

	if (vertexCount == 0)
		vertexCount = m_vertexBufferCount;

	m_pDevice->DrawIndexedPrimitive(s_primitives[primitive], baseVertex, 0, vertexCount, startIndex, primitiveCount);

	m_pAPI->GetRenderStatistic().OnDrawIndexed(primitive, primitiveCount);
}

void Context::DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned int* pIndexData, const void* pVertexData, unsigned int vertexStride)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, vertex count=%d, primitive count=%d, index data ptr=%X, vertex data ptr=%X, vertex stride=%d",
		primitive, vertexCount, primitiveCount, pIndexData, pVertexData, vertexStride);

	m_pDevice->DrawIndexedPrimitiveUP(s_primitives[primitive], 0, vertexCount, primitiveCount, pIndexData, D3DFMT_INDEX32, pVertexData, vertexStride);

	m_pAPI->GetRenderStatistic().OnDrawIndexedUp(primitive, primitiveCount);
}

void Context::DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned short* pIndexData, const void* pVertexData, unsigned int vertexStride)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, vertex count=%d, primitive count=%d, index data ptr=%X, vertex data ptr=%X, vertex stride=%d",
		primitive, vertexCount, primitiveCount, pIndexData, pVertexData, vertexStride);

	m_pDevice->DrawIndexedPrimitiveUP(s_primitives[primitive], 0, vertexCount, primitiveCount, pIndexData, D3DFMT_INDEX16, pVertexData, vertexStride);

	m_pAPI->GetRenderStatistic().OnDrawIndexedUp(primitive, primitiveCount);
}

bool Context::UpdateTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, src ptr=%X dst ptr=%X", src, dst);

	if (src == NULL || dst == NULL ||
		dst->IsRenderTexture() ||
		src->GetWidth() != dst->GetWidth() ||
		src->GetHeight() != dst->GetHeight() ||
		src->GetFormat() != dst->GetFormat())
	{
		return false;
	}

	//���ȳ�����UpdateTexture��ŪŪ
	AutoR<::TextureSurface> pSurfaceSrc = (::TextureSurface*)src->GetSurface(0);
	AutoR<::TextureSurface> pSurfaceDst = (::TextureSurface*)dst->GetSurface(0);
	if (pSurfaceSrc.IsNotNullPtr() && pSurfaceDst.IsNotNullPtr())
	{
		if (S_OK == m_pDevice->UpdateSurface(
			pSurfaceSrc->GetD3DTextureSurfacePtr(), NULL,
			pSurfaceDst->GetD3DTextureSurfacePtr(), NULL))
		{
			dst->GenerateMipmaps();
			return true;
		}
	}

	//���о�ʹ�����ݿ���
	bool successed = false;
	RenderAPI::MappedResource srcLocked = src->LockRect(0, RenderAPI::LOCK_ReadOnly);
	if (srcLocked.Success)
	{
		RenderAPI::MappedResource dstLocked = dst->LockRect(0, RenderAPI::LOCK_Discard);
		if (dstLocked.Success)
		{
			unsigned int height = src->GetHeight();
			char* pSrcDataPtr = reinterpret_cast<char*>(srcLocked.DataPtr);
			char* pDstDataPtr = reinterpret_cast<char*>(dstLocked.DataPtr);
			for (unsigned int row = 0; row < height; ++row)
			{
				memcpy(pDstDataPtr, pSrcDataPtr, dstLocked.LinePitch);
				pSrcDataPtr += srcLocked.LinePitch;
				pDstDataPtr += dstLocked.LinePitch;
			}
			dst->UnlockRect(0);
			dst->GenerateMipmaps();
		}
		src->UnlockRect(0);
	}

	return successed;
}

void Context::InitDeviceCaps(const D3DCAPS9& d3dcaps)
{
	m_deviceCaps.MaxTextureWidth = d3dcaps.MaxTextureWidth;
	m_deviceCaps.MaxTextureHeight = d3dcaps.MaxTextureHeight;
	m_deviceCaps.MaxAnisotropy = d3dcaps.MaxAnisotropy;
	m_deviceCaps.MaxTextureStage = d3dcaps.MaxTextureBlendStages;
	m_deviceCaps.MaxSimultaneousTextures = d3dcaps.MaxSimultaneousTextures;
	m_deviceCaps.MaxUserClipPlanes = d3dcaps.MaxUserClipPlanes;
	m_deviceCaps.MaxPrimitiveCount = d3dcaps.MaxPrimitiveCount;
	m_deviceCaps.MaxVertexIndex = d3dcaps.MaxVertexIndex;
	m_deviceCaps.MaxStreams = d3dcaps.MaxStreams;
	m_deviceCaps.MaxStreamStride = d3dcaps.MaxStreamStride;
	m_deviceCaps.MaxVertexShaderConsts = d3dcaps.MaxVertexShaderConst;
	m_deviceCaps.MaxMRTs = d3dcaps.NumSimultaneousRTs;
	m_deviceCaps.MaxVertexShaderInstruction = d3dcaps.MaxVShaderInstructionsExecuted;
	m_deviceCaps.MaxPixelShaderInstruction = d3dcaps.MaxPShaderInstructionsExecuted;
	m_deviceCaps.MaxVertexBlendMatrix = d3dcaps.MaxVertexBlendMatrices;
	m_deviceCaps.MaxVertexBlendMatrixIndex = d3dcaps.MaxVertexBlendMatrixIndex;
	m_deviceCaps.VertexShaderVersion = D3DSHADER_VERSION_MAJOR(d3dcaps.VertexShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(d3dcaps.VertexShaderVersion);
	m_deviceCaps.PixelShaderVersion = D3DSHADER_VERSION_MAJOR(d3dcaps.PixelShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(d3dcaps.PixelShaderVersion);
	m_deviceCaps.SupportIndex32 = d3dcaps.MaxVertexIndex > 0x0000FFFF;
	m_deviceCaps.SupportsDynamicTexture = (d3dcaps.Caps2&D3DCAPS2_DYNAMICTEXTURES) != 0;
	m_deviceCaps.SupportTextureAlphaChannel = (d3dcaps.TextureCaps&D3DPTEXTURECAPS_ALPHA) != 0;
	m_deviceCaps.SupportOnlySquareTexture = (d3dcaps.TextureCaps&D3DPTEXTURECAPS_SQUAREONLY) != 0;

	if (d3dcaps.TextureCaps&D3DPTEXTURECAPS_POW2)
	{
		if (d3dcaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
		{
			m_deviceCaps.NonePOW2Support = RenderAPI::POW2_Conditional;
		}
		else
		{
			m_deviceCaps.NonePOW2Support = RenderAPI::POW2_None;
		}
	}
	else
	{
		if ((d3dcaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL) == 0)
		{
			m_deviceCaps.NonePOW2Support = RenderAPI::POW2_Support;
		}
	}

	m_deviceCaps.InitVideoMemory = 0;
	if (m_pAPI->GetVendorID() != 0x8086)
	{
		DDrawX ddrawX;
		m_deviceCaps.InitVideoMemory = ddrawX.GetAvailableVideoMemory();
	}
	if (m_deviceCaps.InitVideoMemory == 0)
	{
		m_deviceCaps.InitVideoMemory = GetAvailableTextureMemory();
	}
}

bool Context::StretchTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst, RenderAPI::SamplerFilter filter)
{
	LOG_FUNCTION_V(*m_pAPI, "primitive=%d, src ptr=%X dst ptr=%X, filter=%d", src, dst, filter);

	AutoR<::TextureSurface> pSurfaceSrc = (::TextureSurface*)src->GetSurface(0);
	AutoR<::TextureSurface> pSurfaceDst = (::TextureSurface*)dst->GetSurface(0);

	bool rst = S_OK == m_pDevice->StretchRect(
		pSurfaceSrc->GetD3DTextureSurfacePtr(), NULL,
		pSurfaceDst->GetD3DTextureSurfacePtr(), NULL,
		s_d3dSamplerFilter[filter]);

	return rst;
}

RenderAPI::DeviceState Context::Present()
{
	LOG_FUNCTION_CALL(*m_pAPI, RenderAPI::LOG_Debug);

	HRESULT hr = m_pDevice->Present(NULL, NULL, NULL, NULL);
	return DeviceStateMapping(hr);
}

RenderAPI::DeviceState Context::GetState()
{
	if (m_pDeviceEx != NULL)
	{
		// We recommend not to call CheckDeviceState every frame. 
		// Instead, call CheckDeviceState only if the IDirect3DDevice9Ex::PresentEx method returns a failure code.
		// D3D9EX���豸��ʧҪͨ��CheckDeviceState������
		HRESULT hr = m_pDeviceEx->CheckDeviceState(m_pAPI->hDeviceWindow);
		if (hr == D3DERR_DEVICELOST || hr == D3DERR_DRIVERINTERNALERROR)
		{
			m_pAPI->pDevice->ReleaseDefaultSwapChainWhenLost();
		}
		return DeviceStateMapping(hr);
	}
	else
	{

		HRESULT hr = m_pDevice->TestCooperativeLevel();
		return DeviceStateMapping(hr);
	}
}

RenderAPI::DeviceState Context::ResetDevice(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync)
{
	LOG_FUNCTION_CALL(*m_pAPI, RenderAPI::LOG_Debug);
	HRESULT hr = S_OK;

	D3DPRESENT_PARAMETERS CreationParam = APIInstance::FillCreationParam(
		*m_pAPI,
		(HWND)desc.hWindow,
		desc.backbufferWidth,
		desc.backbufferHeight,
		isFullscreen,
		useVerticalSync,
		s_RTFormats[desc.backbufferFormat],
		s_DSFormats[desc.zbufferFormat],
		s_sampleTypes[desc.aaMode]);

	if (m_pDeviceEx != NULL)
	{
		D3DDISPLAYMODEEX* pfullScreenSetting = NULL;
		D3DDISPLAYMODEEX fullScreenSetting;

		if (CreationParam.Windowed == FALSE)
		{
			fullScreenSetting.Size = sizeof(fullScreenSetting);
			fullScreenSetting.Width = CreationParam.BackBufferWidth;
			fullScreenSetting.Height = CreationParam.BackBufferHeight;
			fullScreenSetting.RefreshRate = CreationParam.FullScreen_RefreshRateInHz;
			fullScreenSetting.Format = CreationParam.BackBufferFormat;
			fullScreenSetting.ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;
			pfullScreenSetting = &fullScreenSetting;
		}
		hr = ((IDirect3DDevice9Ex*)m_pDevice)->ResetEx(&(CreationParam), pfullScreenSetting);
	}
	else
	{
		hr = m_pDevice->Reset(&(CreationParam));
	}

	if (hr == D3DERR_INVALIDCALL)
	{
		return GetState();
	}
	else
	{
		RenderAPI::DeviceState state = DeviceStateMapping(hr);
		if (state == RenderAPI::DEVICE_OK)
		{
			m_pAPI->pDevice->ResetDefaultBackBuffer(
				desc.backbufferWidth,
				desc.backbufferHeight,
				desc.backbufferFormat,
				desc.zbufferFormat
			);

			RenderAPI::SwapChain* pSwapChain = m_pAPI->pDevice->GetDefaultSwapChain();
			RenderAPI::RenderTarget* pRenderTarget = pSwapChain->GetRenderTarget();
			RenderAPI::DepthStencil* pDepthStencil = pSwapChain->GetDepthStencil();
			m_backBufferManager.ResetDefaultRenderTarget(pRenderTarget);
			m_backBufferManager.ResetDefaultDepthStencil(pDepthStencil);
			pRenderTarget->Release();
			pDepthStencil->Release();
			pSwapChain->Release();
		}
		return state;
	}
}

void Context::EvictManagedResources()
{
	LOG_FUNCTION_CALL(*m_pAPI, RenderAPI::LOG_Debug);
	m_pDevice->EvictManagedResources();
}

const RenderAPI::RenderStatisticsData& Context::GetRenderStatisticsData() const
{
	return m_pAPI->GetRenderStatistic().GetData();
}

void Context::ClearRenderStatisticsData()
{
	m_pAPI->GetRenderStatistic().Reset();
}

unsigned int Context::AddReference()
{
	return ++m_refCount;
}

void Context::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}

ID3DXEffectStateManager* Context::GetStateManager()
{
	return &m_renderStateManager;
}

void* Context::GetImplementPtr()
{
	return m_pDevice;
}

bool operator<(const D3DVERTEXELEMENT9& left, const D3DVERTEXELEMENT9& right)
{
	return memcmp(&left, &right, sizeof(D3DVERTEXELEMENT9)) < 0;
}

BackBufferManager::BackBufferManager(IDirect3DDevice9 * device, RenderAPI::RenderTarget * defRT, RenderAPI::DepthStencil * defDS, RenderStatistic& renderStatistic, RenderAPI::Logger& logger)
	: m_internalLogger(logger)
	, m_pDevice(device)
	, m_pCurrentRTs(1)
	, m_pDefaultRT(NULL)
	, m_pDefaultDS(NULL)
	, m_renderStatistic(renderStatistic)
{
	m_pDevice->AddRef();
	m_pCurrentRTs[0] = ((::RenderTarget*)defRT)->GetD3DSurface();
	m_pCurrentDS = ((::DepthStencil*)defDS)->GetD3DSurface();

	m_pDefaultRT = m_pCurrentRTs[0];
	m_pDefaultDS = m_pCurrentDS;
}

BackBufferManager::~BackBufferManager()
{
	m_pDevice->Release();
}

void BackBufferManager::SetRenderTarget(unsigned int index, RenderAPI::RenderTarget * rt)
{
	LOG_FUNCTION_V(m_internalLogger, "index=%d, render target=%X", index, rt);

	//��indexΪ0ʱ��rtΪ�յĺ��屻��Ϊʹ���������Ĭ��backbuffer
	//��Ϊmanager�ж�rt��Cache������������defaultRT������NULL��Ϊ����ʹ��
	//������������NULL�滻ΪdefaultRT
	IDirect3DSurface9* rtSurface;
	if (rt == NULL)
	{
		if (index == 0)
		{
			rtSurface = m_pDefaultRT;
		}
		else
		{
			rtSurface = NULL;
		}

		LOG_FUNCTION_V(m_internalLogger, "switch render target to default rt since input is null");
	}
	else
	{
		rtSurface = ((::RenderTarget*)rt)->GetD3DSurface();
	}

	//������������һ��render texture�Ļ����п��ܱ���Ϊ�������õ�device��
	//������Ҫ������render target֮ǰ�������״̬
	if (rt != NULL)
	{
		::Texture2D* texture = (::Texture2D*)(rt->GetTexturePtr());
		if (texture != NULL)
		{
			IDirect3DBaseTexture9* pTexture;
			for (unsigned int i = 0; i < m_maxTextureStage; i++)
			{
				m_pDevice->GetTexture(i, &pTexture);
				if (pTexture == texture->GetD3DTexture())
				{
					m_pDevice->SetTexture(i, NULL);
					m_renderStatistic.OnSetTexture(NULL);
				}
			}

			texture->Release();
		}
	}

	if (index >= m_pCurrentRTs.size())
	{
		LOG_FUNCTION_V(m_internalLogger, "index=%d, render target=%X", index, rt);
		EnlargeCurrentRTVector(index + 1);
		m_pDevice->SetRenderTarget(index, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
	else if (rtSurface != m_pCurrentRTs[index])
	{
		LOG_FUNCTION_V(m_internalLogger, "index=%d, render target=%X, old=%X", index, rt, m_pCurrentRTs[index]);

		HRESULT hr = m_pDevice->SetRenderTarget(0, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
}

void BackBufferManager::SetDepthStencil(RenderAPI::DepthStencil * ds)
{
	LOG_FUNCTION_V(m_internalLogger, "depth stencil=%X", ds);

	IDirect3DSurface9* dsSurface;
	if (ds == NULL)
	{
		dsSurface = m_pDefaultDS;
	}
	else
	{
		dsSurface = ((::DepthStencil*)ds)->GetD3DSurface();
	}

	if (dsSurface != m_pCurrentDS)
	{
		LOG_FUNCTION_V(m_internalLogger, "depth stencil=%X, old=%X", ds, m_pCurrentDS);
		m_pDevice->SetDepthStencilSurface(dsSurface);
		m_pCurrentDS = dsSurface;
	}
}

bool BackBufferManager::IsDefaultRT()
{
	return m_pDefaultRT == m_pCurrentRTs[0];
}

bool BackBufferManager::IsDefaultDS()
{
	return m_pDefaultDS == m_pCurrentDS;
}

void BackBufferManager::ResetDefaultDepthStencil(RenderAPI::DepthStencil * defDS)
{
	LOG_FUNCTION_V(m_internalLogger, "depth stencil=%X", defDS);

	m_pCurrentDS = ((::DepthStencil*)defDS)->GetD3DSurface();
	m_pDefaultDS = m_pCurrentDS;
}

void BackBufferManager::ResetDefaultRenderTarget(RenderAPI::RenderTarget * defRT)
{
	LOG_FUNCTION_V(m_internalLogger, "render target=%X", defRT);

	m_pCurrentRTs[0] = ((::RenderTarget*)defRT)->GetD3DSurface();
	m_pDefaultRT = m_pCurrentRTs[0];
}

void BackBufferManager::SetMaxTextureStage(unsigned int maxStage)
{
	m_maxTextureStage = maxStage;
}

void BackBufferManager::EnlargeCurrentRTVector(unsigned int count)
{
	size_t oldSize = m_pCurrentRTs.size();
	size_t newSize = count;
	for (; oldSize < newSize; oldSize++)
	{
		m_pCurrentRTs.push_back(NULL);
	}
}

bool operator != (const RenderAPI::VertexElement& left, const RenderAPI::VertexElement& right)
{
	return  (left.SemanticName != right.SemanticName ||
		left.SemanticIndex != right.SemanticIndex ||
		left.AlignOffset != right.AlignOffset ||
		left.Format != right.Format);
}