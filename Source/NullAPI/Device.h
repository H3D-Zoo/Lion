#pragma once

#include "../../RenderAPI/RenderAPI.h"
#include "SwapChain.h"

class Device : public RenderAPI::Device
{
public:
	Device(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual RenderAPI::SwapChain* GetDefaultSwapChain() const;

	virtual RenderAPI::SwapChain* CreateAdditionalSwapChain(const RenderAPI::SwapChainDesc&);

	virtual RenderAPI::VertexBuffer* CreateVertexBuffer(RenderAPI::ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, RenderAPI::Semantic* semantics, unsigned int semanticCount, void* initialData);

	virtual RenderAPI::IndexBuffer* CreateIndexBuffer(RenderAPI::ResourceUsage usage, RenderAPI::IndexFormat format, unsigned int indexCount, void* initialData);

	virtual RenderAPI::Texture2D* CreateTexture2D(RenderAPI::ResourceUsage usage, RenderAPI::TextureFormat format, RenderAPI::TextureBinding binding, unsigned int width, unsigned int height, void* initialData);

	virtual RenderAPI::FXEffect* CreateFXEffectFromFile(const char* effectFilePath);

	virtual RenderAPI::RenderTarget* CreateRenderTarget(RenderAPI::BackBufferFormat format, unsigned int width, unsigned int height);

	virtual RenderAPI::DepthStencil* CreateDepthStencil(RenderAPI::ZBufferFormat format, unsigned int width, unsigned int height);

	virtual void Release();

private:
	mutable ::SwapChain m_deafualtSwapChain;
};