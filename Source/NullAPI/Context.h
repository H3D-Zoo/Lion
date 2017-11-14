#pragma once
#include <vector>
#include <map>
#include "../../RenderAPI/RenderAPI.h"
#include "RenderStatistic.h"
#include "RefCount.hpp"

class APIInstance;
class VertexBuffer;
class IndexBuffer;

class BackBufferManager
{
public:
	BackBufferManager(RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS, RenderStatistic& renderStatistic);
	
	void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* rt);

	void SetDepthStencil(RenderAPI::DepthStencil* rt);

	bool IsDefaultRT();

	bool IsDefaultDS();

	void ResetDefaultDepthStencil(RenderAPI::DepthStencil* defRT);

	void ResetDefaultRenderTarget(RenderAPI::RenderTarget* defDS);

	void SetMaxTextureStage(unsigned int maxStage);

private:
	void EnlargeCurrentRTVector(unsigned int count);

	RenderStatistic& m_renderStatistic;
	std::vector<RenderAPI::RenderTarget*> m_pCurrentRTs;
	RenderAPI::DepthStencil* m_pCurrentDS;
	RenderAPI::RenderTarget* m_pDefaultRT;
	RenderAPI::DepthStencil* m_pDefaultDS;
	unsigned int m_maxTextureStage;
};

class Context :public RenderAPI::Context, public RenderAPI::ContextLegacy
{
public:
	Context(RenderAPI::RenderTarget* defRT, RenderAPI::DepthStencil* defDS,  RenderStatistic& renderStatic);

	virtual const RenderAPI::DeviceCaps& GetDeviceCaps() const;

	virtual unsigned int GetAvailableTextureMemory() const;

	virtual void ClearRenderTarget(unsigned int color);

	virtual void ClearDepthBuffer(float z);

	virtual void ClearStencilBuffer(unsigned int stencil);

	virtual void SetViewport(const RenderAPI::Viewport&);

	virtual RenderAPI::Viewport GetViewport();

	virtual void SetRenderTarget(unsigned int index, RenderAPI::RenderTarget* renderTarget);

	virtual void SetDepthStencil(RenderAPI::DepthStencil* depthStencil);

	virtual void SetVertexBuffers(RenderAPI::VertexBufferInfo* buffers, unsigned int bufferCount);

	virtual void SetIndexBuffer(RenderAPI::IndexBuffer* buffer, unsigned int offset);

	virtual void SetVertexDeclaration(RenderAPI::VertexDeclaration* decl);

	virtual void SetTexture(unsigned int slot, RenderAPI::Texture* texture);

	virtual void SetBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetBlendState() const;

	virtual void SetAlphaSeparateBlendState(const RenderAPI::BlendState& state);

	virtual RenderAPI::BlendState GetAlphaSeparateBlendState() const;

	virtual void SetAlphaTestingState(const RenderAPI::AlphaTestingState& state);

	virtual RenderAPI::AlphaTestingState GetAlphaTestingState() const ;

	virtual void SetDepthTestingState(const RenderAPI::DepthTestingState& state);

	virtual RenderAPI::DepthTestingState GetDepthTestingState() const;

	virtual void SetClipPlaneState(bool isEnable);

	virtual bool GetClipPlaneState() const;

	virtual void SetStencilTestingState(const RenderAPI::StencilTestingState& state);

	virtual RenderAPI::StencilTestingState GetStencilTestingState() const;

	virtual void SetDepthWriting(bool enable);

	virtual void SetTextureSampler(unsigned int slot, const RenderAPI::TextureSampler& sampler);

	virtual void SetScissorState(const RenderAPI::ScissorState& state);

	virtual RenderAPI::ScissorState GetScissorState() const;

	virtual void SetColorWriteMask(bool r, bool g, bool b, bool a);

	virtual void SetFillMode(RenderAPI::FillMode mode);

	virtual RenderAPI::FillMode GetFillMode() const;

	virtual void SetCullMode(RenderAPI::CullMode mode);

	virtual RenderAPI::CullMode GetCullMode() const;

	virtual void SetDepthBias(float bias);

	virtual void SetSlopScaleDepthBias(float bias);

	virtual void SetTextureFactor(unsigned int factor);

	virtual bool BeginScene();

	virtual void EndScene();

	virtual void Draw(RenderAPI::Primitive primitive, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int primitiveCount, const void* pVertexData, unsigned int vertexStride);

	virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount);

	virtual void DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned int* pIndexData, const void* pVertexData, unsigned int vertexStride);

	virtual void DrawIndexedWithDynamicVertex(RenderAPI::Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned short* pIndexData, const void* pVertexData, unsigned int vertexStride);

	virtual bool UpdateTexture(RenderAPI::Texture2D* src, RenderAPI::Texture2D* dst);

	virtual bool StretchTexture(RenderAPI::Texture2D* src, RenderAPI::Texture2D* dst, RenderAPI::SamplerFilter filter);

	virtual RenderAPI::DeviceState Context::Present();

	virtual RenderAPI::DeviceState GetState();

	virtual RenderAPI::DeviceState ResetDevice(const RenderAPI::SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	virtual void EvictManagedResources();
	
	virtual const RenderAPI::RenderStatisticsData& GetRenderStatisticsData() const;
	
	virtual void ClearRenderStatisticsData();

	virtual RenderAPI::ContextLegacy* GetContextLegacy();

	virtual unsigned int AddReference();

	virtual void Release();

	// Context Legacy
	virtual void* GetImplementPtr();

	virtual void SetTextureColorBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	virtual void SetTextureAlphaBlendingState(unsigned int slot, const RenderAPI::TextureBlendingState& state);

	virtual void SetTexcoordTransform(unsigned int slot, RenderAPI::TextureTransform transform);

	virtual void ResetTexcoordIndex(unsigned int slot);

	virtual void DisableCustomShaderUsage();

	virtual void SetCustomFVF(unsigned int fvf);

	virtual void SetWorldMatrix(const float* matrix);

	virtual void SetViewMatrix(const float* matrix);

	virtual void SetProjectionMatrix(const float* matrix);

	virtual void SetTextureMatrix(unsigned int slot, const float* matrix);

	virtual void SetFog(const RenderAPI::FogSetting& fog);

	virtual void SetLight(const RenderAPI::PointLight& pLight);

	virtual void SetLight(const RenderAPI::DirectionalLight& pLight);

	virtual void DisableLight();

	virtual void SetMaterial(const RenderAPI::Material& mat);

	virtual void GenPerspectiveMatrixRH(float outMatrix[16], float fovRadian, float aspect, float nearZ, float farZ);

	virtual void GenOrthoCenterMatrixRH(float outMatrix[16], float width, float height, float nearZ, float farZ);

	virtual void GenOrthoOffCenterMatrixRH(float outMatrix[16], float left, float right, float bottom, float top, float nearZ, float farZ);

	virtual void GenViewMatrix(float outMatrix[16], const RenderAPI::Float3& eye, const RenderAPI::Float3& lookAt, const RenderAPI::Float3& upward);

	virtual void ProjectVertexPos(RenderAPI::Float3& inoutPos, const float matMV[16], const float matP[16], RenderAPI::Viewport viewport);

	virtual void SaveNXDebugRenderState();

	virtual void RestoreNXDebugRenderState(bool lightsOn);

private:
	RefCount m_refCount;
	BackBufferManager m_backBufferManager;
	RenderAPI::DeviceCaps m_deviceCaps;
	unsigned int m_indexBufferOffset;
	unsigned int m_vertexBufferCount;
	mutable RenderAPI::ScissorState m_scissorState;
	DWORD m_nNXCacheFVF;
	RenderStatistic& m_renderStatistic;

	RenderAPI::Viewport m_currentViewport;

	RenderAPI::BlendState m_currentBlendState;
	RenderAPI::BlendState m_currentAlphaSeparateBlendState;
	RenderAPI::AlphaTestingState m_currentAlphaTestingState;
	RenderAPI::DepthTestingState m_currentDepthTestingState;
	RenderAPI::StencilTestingState m_currentStencilTestingState;
	RenderAPI::ScissorState m_currentScissorState;
	RenderAPI::FillMode m_currentFillMode;
	RenderAPI::CullMode m_currentCullMode;
	bool m_currentClipPlaneState;
	bool m_currentDepthWritingState;
};