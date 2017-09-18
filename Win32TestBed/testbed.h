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
	RenderAPI::APIInstance* m_pAPIInstance = nullptr;
	RenderAPI::Device* m_pDevice = nullptr;
	RenderAPI::Context* m_pContext = nullptr;
	RenderAPI::SwapChain* m_defaultSwapChain = nullptr;
	RenderAPI::SwapChain* m_editorSwapChain = nullptr;

	typedef RenderAPI::APIInstance* (*CreateAPIGlobal)();
	CreateAPIGlobal m_createAPIInstance = nullptr;

	RenderAPI::Viewport m_bbViewport;

	struct Mesh
	{
		std::vector<RenderAPI::VertexBufferInfo> vertex;
		RenderAPI::IndexBuffer* index;
		RenderAPI::VertexDeclaration* declaration;
		void Set(RenderAPI::Context*);
		void Release();
	};

	Mesh Particle;
	Mesh Box;
	Mesh Quad;

	RenderAPI::FXEffect* m_pEffectTintColor = nullptr;
	RenderAPI::FXEffect* m_pEffectParticle = nullptr;
	RenderAPI::FXEffect* m_pEffectSimpleTexture = nullptr;

	RenderAPI::Texture2D* m_pParticleTexture = nullptr;
	RenderAPI::Texture2D* m_pBoxTexture = nullptr;

	RenderAPI::RenderTarget* m_pRenderTexture = nullptr;
	RenderAPI::DepthStencil* m_pRenderDepth = nullptr;

	ParticleInstance m_particleInstance;

	gml::vec4 m_texelOffsetOffScreen;
	gml::vec4 m_textureSize;
	gml::mat44 m_matWorldParticle;
	std::vector<gml::mat44> m_matWorldBoxs;
	gml::mat44 m_matView;
	gml::mat44 m_matProj;
	gml::mat44 m_matProjRTT;
	gml::mat44 m_matInvView;

	RenderAPI::HEffectParam m_hTechStencil;
	RenderAPI::HEffectParam m_hTechSimple;

	RenderAPI::HEffectParam m_hParamTexture;
	RenderAPI::HEffectParam m_hParamParticleTex;
};