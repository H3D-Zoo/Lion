#pragma once
#include <Windows.h>
#include <RenderAPI.h>
#include <vector>
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
	void CreateMaterial();
	void DoEffectDraw();

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

	RenderAPI::VertexBuffer* m_pBoxVertexBuffer = nullptr;
	RenderAPI::IndexBuffer* m_pBoxIndexBuffer = nullptr;
	RenderAPI::FXEffect* m_pEffect = nullptr;
	std::vector<RenderAPI::VertexBufferInfo> m_vertexBufferInfos;
};