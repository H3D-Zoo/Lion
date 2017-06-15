#include "testbed.h"

#define DLLName "NullAPI.dll"

bool APITestBed::Init(HWND hWindow, unsigned int backBufferWidth, unsigned int backBufferHeight)
{
	m_hRenderAPIDLL = LoadLibraryA(DLLName);
	if (m_hRenderAPIDLL == nullptr)
	{
		return false;
	}

	m_apiInitPtr = (RenderAPIInit)GetProcAddress(m_hRenderAPIDLL, "Initialize");
	m_apiDeinitPtr = (RenderAPIDeinit)GetProcAddress(m_hRenderAPIDLL, "Deinitialize");
	m_apiCreatePtr = (RenderAPICreate)GetProcAddress(m_hRenderAPIDLL, "CreateDeviceAndContext");

	if (m_apiInitPtr == nullptr ||
		m_apiDeinitPtr == nullptr ||
		m_apiCreatePtr == nullptr)
	{
		FreeLibrary(m_hRenderAPIDLL);
		m_hRenderAPIDLL = nullptr;
		return false;
	}

	if (!m_apiInitPtr())
	{
		return false;
	}


	RenderAPI::SwapChainDesc desc;
	desc.hWindow = hWindow;
	desc.backbufferFormat = RenderAPI::BACKBUFFER_XRGB8;
	desc.zbufferFormat = RenderAPI::ZBUFFER_D24S8;
	desc.aaMode = RenderAPI::AA_Disable;
	desc.backbufferWidth = backBufferWidth;
	desc.backbufferHeight = backBufferHeight;
	auto rst = m_apiCreatePtr(desc, false, false);

	if (!rst.Success)
	{
		return false;
	}
	m_pDevice = rst.DevicePtr;
	m_pContext = rst.ContextPtr;
	m_defaultSwapChain = rst.DevicePtr->GetDefaultSwapChain();
	if (m_defaultSwapChain == nullptr)
	{
		return false;
	}
	return true;
}

void APITestBed::Deinit()
{
	if (m_hRenderAPIDLL != nullptr)
	{
		if (m_defaultSwapChain != nullptr)
		{
			m_defaultSwapChain->Release();
			m_defaultSwapChain = nullptr;
		}
		if (m_pContext != nullptr)
		{
			m_pContext->Release();
			m_pContext = nullptr;
		}
		if (m_pDevice != nullptr)
		{
			m_pDevice->Release();
			m_pDevice = nullptr;
		}

		m_apiDeinitPtr();

		FreeLibrary(m_hRenderAPIDLL);
		m_hRenderAPIDLL = nullptr;
	}
}

void APITestBed::Update()
{
	m_pContext->ClearRenderTarget(m_defaultSwapChain->GetRenderTarget(), 0xFF00FF00);
	m_defaultSwapChain->Present();
}

void APITestBed::OnResize(unsigned int width, unsigned int height)
{
	m_defaultSwapChain->OnResize(width, height);
}
