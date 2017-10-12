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
#include <ddraw.h>

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
	IDirectDraw7* m_pDD7;
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

	D3DTEXTUREFILTERTYPE s_d3dSamplerFilter[] =
	{
		D3DTEXF_NONE,
		D3DTEXF_POINT,
		D3DTEXF_LINEAR,
		D3DTEXF_ANISOTROPIC,
	};
}

Context::Context(APIInstance* pAPI, IDirect3DDevice9 * device, RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS)
	: m_pAPI(pAPI)
	, m_pDevice(device)
	, m_backBufferManager(device, defRT, defDS)
	, m_renderStateManager(device)
{
	m_pAPI->pContext = this;
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

	RenderAPI::DeviceCaps caps = GetDeviceCaps();
	m_backBufferManager.SetMaxTextureStage(caps.MaxTextureStage);
}

Context::~Context()
{
	m_pDevice->Release();
	m_pAPI->Release();
	m_pDeviceEx = NULL;
	m_pDevice = NULL;
	m_pAPI = NULL;
}

RenderAPI::DeviceCaps Context::GetDeviceCaps()
{
	//��ȡD3D��Ϣ
	D3DCAPS9 d3dcaps;
	RenderAPI::DeviceCaps caps;
	m_pDevice->GetDeviceCaps(&d3dcaps);

	caps.MaxTextureWidth = d3dcaps.MaxTextureWidth;
	caps.MaxTextureHeight = d3dcaps.MaxTextureHeight;
	caps.MaxAnisotropy = d3dcaps.MaxAnisotropy;
	caps.MaxTextureStage = d3dcaps.MaxTextureBlendStages;
	caps.MaxSimultaneousTextures = d3dcaps.MaxSimultaneousTextures;
	caps.MaxUserClipPlanes = d3dcaps.MaxUserClipPlanes;
	caps.MaxPrimitiveCount = d3dcaps.MaxPrimitiveCount;
	caps.MaxVertexIndex = d3dcaps.MaxVertexIndex;
	caps.MaxStreams = d3dcaps.MaxStreams;
	caps.MaxStreamStride = d3dcaps.MaxStreamStride;
	caps.MaxVertexShaderConsts = d3dcaps.MaxVertexShaderConst;
	caps.MaxMRTs = d3dcaps.NumSimultaneousRTs;
	caps.MaxVertexShaderInstruction = d3dcaps.MaxVShaderInstructionsExecuted;
	caps.MaxPixelShaderInstruction = d3dcaps.MaxPShaderInstructionsExecuted;
	caps.MaxVertexBlendMatrix = d3dcaps.MaxVertexBlendMatrices;
	caps.MaxVertexBlendMatrixIndex = d3dcaps.MaxVertexBlendMatrixIndex;


	caps.VertexShaderVersion = D3DSHADER_VERSION_MAJOR(d3dcaps.VertexShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(d3dcaps.VertexShaderVersion);
	caps.PixelShaderVersion = D3DSHADER_VERSION_MAJOR(d3dcaps.PixelShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(d3dcaps.PixelShaderVersion);

	caps.SupportIndex32 = d3dcaps.MaxVertexIndex > 0x0000FFFF;
	caps.SupportsDynamicTexture = (d3dcaps.Caps2&D3DCAPS2_DYNAMICTEXTURES) != 0;
	caps.SupportTextureAlphaChannel = (d3dcaps.TextureCaps&D3DPTEXTURECAPS_ALPHA) != 0;
	caps.SupportOnlySquareTexture = (d3dcaps.TextureCaps&D3DPTEXTURECAPS_SQUAREONLY) != 0;

	if (d3dcaps.TextureCaps&D3DPTEXTURECAPS_POW2)
	{
		if (d3dcaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
		{
			caps.NonePOW2Support = RenderAPI::POW2_Conditional;
		}
		else
		{
			caps.NonePOW2Support = RenderAPI::POW2_None;
		}
	}
	else
	{
		if ((d3dcaps.TextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL) == 0)
		{
			caps.NonePOW2Support = RenderAPI::POW2_Support;
		}
	}

	caps.InitVideoMemory = 0;
	if (m_pAPI->GetVendorID() != 0x8086)
	{
		DDrawX ddrawX;
		caps.InitVideoMemory = ddrawX.GetAvailableVideoMemory();
	}
	if (caps.InitVideoMemory == 0)
	{
		caps.InitVideoMemory = GetAvailableTextureMemory();
	}
	return caps;
}

unsigned int Context::GetAvailableTextureMemory()
{
	return m_pDevice->GetAvailableTextureMem();
}

void Context::ClearRenderTarget(unsigned int color)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 0, 0);
}

void Context::ClearDepthBuffer(float z)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, z, 0);
}

void Context::ClearStencilBuffer(unsigned int stencil)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, stencil);
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
	m_vertexBufferCount = 0;
	for (unsigned int i = 0; i < bufferCount; i++)
	{
		if (buffers[i].BufferPtr != NULL)
		{
			::VertexBuffer* pVertexBuffe = (::VertexBuffer*)buffers[i].BufferPtr;
			IDirect3DVertexBuffer9* vertexBufferPtr = pVertexBuffe->GetBufferPtr();

			if (m_vertexBufferCount == 0 || m_vertexBufferCount > pVertexBuffe->GetVertexCount())
			{
				m_vertexBufferCount = pVertexBuffe->GetVertexCount();
			}

			HRESULT hr = m_pDevice->SetStreamSource(i, vertexBufferPtr, buffers[i].Offset, buffers[i].BufferPtr->GetVertexStride());
		}
	}
}

void Context::SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset)
{
	if (buffer != NULL)
	{
		IDirect3DIndexBuffer9* indexBufferPtr = ((::IndexBuffer*)buffer)->GetD3DIndexBuffer();
		m_pDevice->SetIndices(indexBufferPtr);
		m_indexBufferOffset = offset;
	}
}

void Context::SetVertexDeclaration(RenderAPI::VertexDeclaration * decl)
{
	if (decl != NULL)
	{
		IDirect3DVertexDeclaration9* pDeclaration = ((::VertexDeclaration*)decl)->GetD3DVertexDeclarationPtr();
		m_pDevice->SetVertexDeclaration(pDeclaration);
	}
}

void Context::SetTexture(unsigned int slot, RenderAPI::Texture* texture)
{
	IDirect3DBaseTexture9* pTexture = NULL;
	if (texture != NULL)
	{
		if (texture->IsCubemap())
			pTexture = ((::TextureCube*)texture)->GetD3DTexture();
		else
			pTexture = ((::Texture2D*)texture)->GetD3DTexture();
	}
	m_pDevice->SetTexture(slot, pTexture);
}

void Context::SetBlendState(const RenderAPI::BlendState& state)
{
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

void Context::SetStencilTestingState(const RenderAPI::StencilTestingState& state)
{
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
	m_renderStateManager.SetDepthWrite(enable ? TRUE : FALSE);
}

void Context::SetTextureColorBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state)
{
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
	DWORD magFilter = s_d3dSamplerFilter[sampler.MagFilter == RenderAPI::FILTER_Anisotropic ? RenderAPI::FILTER_Linear : sampler.MagFilter];
	m_renderStateManager.SetSamplerMinFilter(slot, s_d3dSamplerFilter[sampler.MinFilter]);
	m_renderStateManager.SetSamplerMagFilter(slot, magFilter);
	m_renderStateManager.SetSamplerMipFilter(slot, s_d3dSamplerFilter[sampler.MipFilter]);
	m_renderStateManager.SetSamplerAddressU(slot, s_textureAddress[sampler.AddressU]);
	m_renderStateManager.SetSamplerAddressV(slot, s_textureAddress[sampler.AddressV]);
	m_renderStateManager.SetSamplerAddressW(slot, s_textureAddress[sampler.AddressW]);
	if (sampler.AddressU == RenderAPI::TEX_ADDRESS_Border || sampler.AddressV == RenderAPI::TEX_ADDRESS_Border || sampler.AddressW == RenderAPI::TEX_ADDRESS_Border)
	{
		m_renderStateManager.SetSamplerBorderColor(slot, sampler.BorderColor);
	};
	if (sampler.MagFilter == RenderAPI::FILTER_Anisotropic)
	{
		m_renderStateManager.SetMaxAnisotropy(slot, s_sampleTypes[sampler.OptionalAnisotropicFilter]);
	}
}

void Context::SetScissorState(const RenderAPI::ScissorState& state)
{
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
	m_renderStateManager.SetFillMode(s_fillMode[mode]);
}

void Context::SetCullMode(RenderAPI::CullMode mode)
{
	m_renderStateManager.SetCullMode(s_cullMode[mode]);
}

void Context::SetDepthBias(float bias)
{
	m_renderStateManager.SetDepthBias((DWORD)bias);
}

void Context::SetSlopScaleDepthBias(float bias)
{
	m_renderStateManager.SetSlopScaleDepthBias((DWORD)bias);
}

void Context::SetTextureFactor(unsigned int factor)
{
	m_renderStateManager.SetTextureFactor(factor);
}

bool Context::BeginScene()
{
	return S_OK == m_pDevice->BeginScene();
}

void Context::EndScene()
{
	m_pDevice->EndScene();
}

void Context::Draw(RenderAPI::Primitive primitive, unsigned int startVertex, unsigned int primitiveCount)
{
	m_pDevice->DrawPrimitive(s_primitives[primitive], startVertex, primitiveCount);
}

void Context::DrawWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int primitiveCount, const void* pVertexData, unsigned int vertexStride)
{
	m_pDevice->DrawPrimitiveUP(s_primitives[primitive], primitiveCount, pVertexData, vertexStride);
}

void Context::DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount)
{
	if (vertexCount == 0)
		vertexCount = m_vertexBufferCount;
	m_pDevice->DrawIndexedPrimitive(s_primitives[primitive], baseVertex, 0, vertexCount, startIndex, primitiveCount);
}

void Context::DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned int* pIndexData, const void* pVertexData, unsigned int vertexStride)
{
	m_pDevice->DrawIndexedPrimitiveUP(s_primitives[primitive], 0, vertexCount, primitiveCount, pIndexData, D3DFMT_INDEX32, pVertexData, vertexStride);
}

void Context::DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned short* pIndexData, const void* pVertexData, unsigned int vertexStride)
{
	m_pDevice->DrawIndexedPrimitiveUP(s_primitives[primitive], 0, vertexCount, primitiveCount, pIndexData, D3DFMT_INDEX16, pVertexData, vertexStride);
}

bool Context::UpdateTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst)
{
	if (src == NULL || dst == NULL)
	{
		return false;
	}
	IDirect3DTexture9* pSrcTexture = ((::Texture2D*)src)->GetD3DTexture();
	IDirect3DTexture9* pDstTexture = ((::Texture2D*)dst)->GetD3DTexture();
	return S_OK == m_pDevice->UpdateTexture(pSrcTexture, pDstTexture);
}

bool Context::StretchTexture(RenderAPI::Texture2D * src, RenderAPI::Texture2D * dst, RenderAPI::SamplerFilter filter)
{
	AutoR<::TextureSurface> pSurfaceSrc = (::TextureSurface*)src->GetSurface(0);
	AutoR<::TextureSurface> pSurfaceDst = (::TextureSurface*)dst->GetSurface(0);

	pSurfaceSrc = (::TextureSurface*)src->GetSurface(0);
	bool rst = S_OK == m_pDevice->StretchRect(pSurfaceSrc->GetD3DTextureSurfacePtr(), NULL, pSurfaceDst->GetD3DTextureSurfacePtr(), NULL, s_d3dSamplerFilter[filter]);


	return rst;
}

bool Context::GetRenderTargetData(RenderAPI::RenderTarget * rt, RenderAPI::TextureSurface * surface)
{
	IDirect3DSurface9* pSurfaceRT = ((::RenderTarget*)rt)->GetD3DSurface();
	IDirect3DSurface9* pRTSurfaceDst = ((::TextureSurface*)surface)->GetD3DTextureSurfacePtr();
	return S_OK == m_pDevice->GetRenderTargetData(pSurfaceRT, pRTSurfaceDst);
}

bool Context::GetDepthStencilData(RenderAPI::DepthStencil * ds, RenderAPI::TextureSurface * surface)
{
	IDirect3DSurface9* pSurfaceDS = ((::DepthStencil*)ds)->GetD3DSurface();
	IDirect3DSurface9* pRTSurfaceDst = ((::TextureSurface*)surface)->GetD3DTextureSurfacePtr();
	return S_OK == m_pDevice->GetRenderTargetData(pSurfaceDS, pRTSurfaceDst);
}

RenderAPI::DeviceState Context::Present()
{
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
	m_pDevice->EvictManagedResources();
}

void Context::Release()
{
	delete this;
}

ID3DXEffectStateManager* Context::GetStateManager()
{
	return &m_renderStateManager;
}

bool operator<(const D3DVERTEXELEMENT9& left, const D3DVERTEXELEMENT9& right)
{
	return memcmp(&left, &right, sizeof(D3DVERTEXELEMENT9)) < 0;
}

BackBufferManager::BackBufferManager(IDirect3DDevice9 * device, RenderAPI::RenderTarget * defRT, RenderAPI::DepthStencil * defDS)
	: m_pDevice(device)
	, m_pCurrentRTs(1)
	, m_pDefaultRT(NULL)
	, m_pDefaultDS(NULL)
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
				}
			}
		}
	}


	if (index >= m_pCurrentRTs.size())
	{
		m_pCurrentRTs.resize(index + 1);
		m_pDevice->SetRenderTarget(index, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
	else if (rtSurface != m_pCurrentRTs[index])
	{
		HRESULT hr = m_pDevice->SetRenderTarget(0, rtSurface);
		m_pCurrentRTs[index] = rtSurface;
	}
}

void BackBufferManager::SetDepthStencil(RenderAPI::DepthStencil * ds)
{
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

void BackBufferManager::ResetDefaultDepthStencil(RenderAPI::DepthStencil * defRT)
{
	m_pCurrentRTs[0] = ((::RenderTarget*)defRT)->GetD3DSurface();
	m_pDefaultRT = m_pCurrentRTs[0];
}

void BackBufferManager::ResetDefaultRenderTarget(RenderAPI::RenderTarget * defDS)
{
	m_pCurrentDS = ((::DepthStencil*)defDS)->GetD3DSurface();
	m_pDefaultDS = m_pCurrentDS;
}

void BackBufferManager::SetMaxTextureStage(unsigned int maxStage)
{
	m_maxTextureStage = maxStage;
}

bool operator != (const RenderAPI::VertexElement& left, const RenderAPI::VertexElement& right)
{
	return  (left.SemanticName != right.SemanticName ||
		left.SemanticIndex != right.SemanticIndex ||
		left.AlignOffset != right.AlignOffset ||
		left.Format != right.Format);
}