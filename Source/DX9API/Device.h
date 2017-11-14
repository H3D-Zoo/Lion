#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "SwapChain.h"
#include "DepthStencil.h"
#include "DX9Include.h"
#include "RenderStatistic.h"

class APIInstance;
class Device : public RenderAPI::Device
{
public:
	Device(APIInstance* pAPIContext, IDirect3DDevice9* device, const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	~Device();

	void ReleaseDefaultSwapChainWhenLost();

	void ResetDefaultBackBuffer(unsigned int width, unsigned int height, RenderAPI::RenderTargetFormat rtFormat, RenderAPI::DepthStencilFormat dsFormat);

	virtual RenderAPI::SwapChain* GetDefaultSwapChain();

	virtual RenderAPI::SwapChain* CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc&);

	virtual RenderAPI::VertexBuffer* CreateVertexBuffer(RenderAPI::ResourceUsage usage,unsigned int vertexCount, unsigned int vertexSize);

	virtual RenderAPI::IndexBuffer* CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount);

	virtual RenderAPI::VertexDeclaration* CreateVertexDeclaration(const RenderAPI::VertexElement* elements, unsigned int elementCount);

	virtual RenderAPI::Texture2D* CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps);

	virtual RenderAPI::Texture2D* CreateScaledTexture2D(RenderAPI::ResourceUsage usage, unsigned int width, unsigned int height, char* fileBuffer, unsigned int fileLengh);

	virtual RenderAPI::TextureCube* CreateTextureCube(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, unsigned int edgeLength, unsigned int layer, bool autoGenMipmaps);

	virtual RenderAPI::FXEffect* CreateFXEffectFromFile(const char* effectFilePath, const char * includeDir);

	virtual RenderAPI::RenderTarget* CreateRenderTarget(RenderAPI::TextureFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::DepthStencil* CreateDepthStencil(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::OcclusionQuery* CreateOcclusionQuery();

	virtual RenderAPI::TextBox* CreateTextBox(int screenX, int screenY, int width, int height);

	virtual unsigned int AddReference();

	virtual void Release();

private:
	RefCount m_refCount;
	APIInstance* m_pAPIInstance;
	::DepthStencil* CreateDepthStencilImplement(RenderAPI::DepthStencilFormat format, unsigned int width, unsigned int height);
	::SwapChain* m_pDefaultSwapChain;
	IDirect3DDevice9* m_pDevice;
	bool m_supportDynamicTexture;
};