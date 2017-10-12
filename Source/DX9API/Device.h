#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "SwapChain.h"
#include "DepthStencil.h"
#include "DX9Include.h"


class APIGlobal;
class Device : public RenderAPI::Device
{
public:
	Device(APIGlobal* pAPIContext, IDirect3DDevice9* device, const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	~Device();

	void ReleaseDefaultSwapChainWhenLost();

	void ResetDefaultBackBuffer(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, RenderAPI::DepthStencilFormat dsFormat);

	virtual RenderAPI::SwapChain* GetDefaultSwapChain();

	virtual RenderAPI::SwapChain* CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc&);

	virtual RenderAPI::VertexBuffer* CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, void* initialData);

	virtual RenderAPI::IndexBuffer* CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount, void* initialData);

	virtual RenderAPI::VertexDeclaration* CreateVertexDeclaration(const RenderAPI::VertexElement* elements, unsigned int elementCount);

	virtual RenderAPI::Texture2D* CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, void* initialData, int dataLinePitch, int dataHeight);

	virtual RenderAPI::FXEffect* CreateFXEffectFromFile(const char* effectFilePath);

	virtual RenderAPI::RenderTarget* CreateRenderTarget(RenderAPI::RenderTargetFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::DepthStencil* CreateDepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::OcclusionQuery* CreateOcclusionQuery();

	virtual void* GetImplementPtr();

	virtual void Release();

private:
	APIGlobal* m_pAPI;
	::DepthStencil* CreateDepthStencilImplement(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);
	::SwapChain* m_pDefaultSwapChain;
	IDirect3DDevice9* m_pDevice;
};