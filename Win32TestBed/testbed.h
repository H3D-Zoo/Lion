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
	void CreatePartcleMesh();
	void CreateMaterial();
	void DrawBox(RenderAPI::TextureAddress address);
	void DrawParticle();
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

	RenderAPI::VertexBuffer* m_pParticleVBS = nullptr;
	RenderAPI::VertexBuffer* m_pParticleVBD = nullptr;
	RenderAPI::IndexBuffer* m_pParticleIB = nullptr;

	RenderAPI::VertexBuffer* m_pBoxVertexBuffer = nullptr;
	RenderAPI::IndexBuffer* m_pBoxIndexBuffer = nullptr;

	RenderAPI::FXEffect* m_pEffectTintColor = nullptr;
	RenderAPI::FXEffect* m_pEffectParticle = nullptr;

	RenderAPI::Texture2D* m_pParticleTexture = nullptr;
	RenderAPI::Texture2D* m_pBoxTexture = nullptr;

	std::vector<RenderAPI::VertexBufferInfo> m_boxVBInfos;
	std::vector<RenderAPI::VertexBufferInfo> m_particleVBInfos;

	ParticleInstance m_particleInstance;

	gml::mat44 m_matWorldParticle;
	gml::mat44 m_matWorldBox;
	gml::mat44 m_matView;
	gml::mat44 m_matProj;
	gml::mat44 m_matInvView;

	
};