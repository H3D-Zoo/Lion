#pragma once
#include <Windows.h>
#include <RenderAPI.h>
#include <vector>

#include "Math/gmlvector.h"
#include "Math/gmlmatrix.h"
#include "particle.h"

class APITestBed
{
public:
	bool Init(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight);
	void Deinit();
	void Update();
	void OnResize(unsigned int width, unsigned int height);
	RenderAPI::SwapChain* GetSubWindowSwapChain() { return m_editorSwapChain; }

private:
	bool LoadDLL();
	bool CreateDeviceAndContext(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight);
	void CreateMesh();
	void CreateQuadMesh();
	void CreatePartcleMesh();
	void CreateMaterial();
	
	void DrawBox(RenderAPI::TextureAddress address, bool alphaBlending);
	void DrawParticle(const gml::mat44& matProj);
	void DrawRTTQuad();
	void UploadParticlesAndCommitDrawcalls();

	HMODULE m_hRenderAPIDLL = nullptr;
	RenderAPI::Device* m_pDevice = nullptr;
	RenderAPI::Context* m_pContext = nullptr;
	RenderAPI::SwapChain* m_defaultSwapChain = nullptr;
	RenderAPI::SwapChain* m_editorSwapChain = nullptr;

	typedef bool(*RenderAPIInit)();
	typedef void(*RenderAPIDeinit)();
	typedef RenderAPI::CreationResult(*RenderAPICreate)(const RenderAPI::SwapChainDesc& desc, bool fullscreen, bool vsync);
	RenderAPIInit m_apiInitPtr = nullptr;
	RenderAPIDeinit m_apiDeinitPtr = nullptr;
	RenderAPICreate m_apiCreatePtr = nullptr;

	RenderAPI::Viewport m_bbViewport;

	RenderAPI::VertexBuffer* m_pParticleVBS = nullptr;
	RenderAPI::VertexBuffer* m_pParticleVBD = nullptr;
	RenderAPI::IndexBuffer* m_pParticleIB = nullptr;

	RenderAPI::VertexBuffer* m_pBoxVertexBuffer = nullptr;
	RenderAPI::IndexBuffer* m_pBoxIndexBuffer = nullptr;

	RenderAPI::VertexBuffer* m_pQuadVB = nullptr;
	RenderAPI::IndexBuffer* m_pQuadIB = nullptr;

	RenderAPI::FXEffect* m_pEffectTintColor = nullptr;
	RenderAPI::FXEffect* m_pEffectParticle = nullptr;
	RenderAPI::FXEffect* m_pEffectSimpleTexture = nullptr;

	RenderAPI::Texture2D* m_pParticleTexture = nullptr;
	RenderAPI::Texture2D* m_pBoxTexture = nullptr;

	RenderAPI::RenderTarget* m_pRenderTexture = nullptr;
	RenderAPI::DepthStencil* m_pRenderDepth = nullptr;

	std::vector<RenderAPI::VertexBufferInfo> m_boxVBInfos;
	std::vector<RenderAPI::VertexBufferInfo> m_particleVBInfos;

	ParticleInstance m_particleInstance;

	gml::vec4 m_texelOffsetOffScreen;
	gml::vec4 m_textureSize;
	gml::mat44 m_matWorldParticle;
	std::vector<gml::mat44> m_matWorldBoxs;
	gml::mat44 m_matView;
	gml::mat44 m_matProj;
	gml::mat44 m_matProjRTT;
	gml::mat44 m_matInvView;

	
};