#include "testbed.h"
#include <vector>
#define DLLName "DX9API.dll"

bool APITestBed::Init(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight)
{
	if (!LoadDLL())
	{
		return false;
	}

	if (!m_apiInitPtr())
	{
		return false;
	}

	if (!CreateDeviceAndContext(hWindow, hWindowEditor, backBufferWidth, backBufferHeight))
	{
		return false;
	}

	CreateMesh();
	CreateMaterial();

	const int kMatrixLength = sizeof(float) * 16;
	memset(m_matWorld, 0, kMatrixLength);
	memset(m_matView, 0, kMatrixLength);
	memset(m_matProj, 0, kMatrixLength);

	m_matWorld[0] = m_matWorld[5] = m_matWorld[10] = m_matWorld[15] = 1.0f;
	m_matView[0] = m_matView[5] = m_matView[10] = m_matView[15] = 1.0f;
	m_matView[11] = 10.0f;

	float zRangeInv = 101.0f / 100.0f;
	m_matProj[10] = zRangeInv;
	m_matProj[11] = -zRangeInv;
	m_matProj[14] = 1.0f;
	return true;
}

void APITestBed::Deinit()
{
	if (m_pBoxVertexBuffer)
	{
		m_pBoxVertexBuffer->Release();
		m_pBoxVertexBuffer = NULL;
	}

	if (m_pBoxIndexBuffer)
	{
		m_pBoxIndexBuffer->Release();
		m_pBoxIndexBuffer = NULL;
	}

	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = NULL;
	}

	if (m_editorSwapChain != nullptr)
	{
		m_editorSwapChain->Release();
		m_editorSwapChain = nullptr;
	}

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

	if (m_apiDeinitPtr)
	{
		m_apiDeinitPtr();
		m_apiDeinitPtr = nullptr;
	}

	if (m_hRenderAPIDLL != nullptr)
	{
		FreeLibrary(m_hRenderAPIDLL);
		m_hRenderAPIDLL = nullptr;
	}
}

void APITestBed::Update()
{
	
	auto rt = m_defaultSwapChain->GetRenderTarget();
	auto ds = m_defaultSwapChain->GetDepthStencil();
	m_pContext->SetRenderTarget(0, rt);
	m_pContext->SetDepthStencil(ds);
	m_pContext->SetCullMode(RenderAPI::CULL_CCW);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(rt, 0xFF00FF00);
		m_pContext->ClearDepthStencil(ds, 1.0f, 0);
		DoEffectDraw();
		m_pContext->EndScene();
	}

	m_defaultSwapChain->Present();
	rt->Release();
	ds->Release();

	auto rtEditor = m_editorSwapChain->GetRenderTarget();
	auto dsEditor = m_editorSwapChain->GetDepthStencil();
	m_pContext->SetRenderTarget(0, rtEditor);
	m_pContext->SetDepthStencil(dsEditor);
	m_pContext->SetCullMode(RenderAPI::CULL_None);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(rtEditor, 0xFFFF0000);
		m_pContext->ClearDepthStencil(dsEditor, 1.0f, 0);
		DoEffectDraw();
		m_pContext->EndScene();
	}
	m_editorSwapChain->Present();
	rtEditor->Release();
	dsEditor->Release();

	if (m_pContext->CheckDeviceLost() == RenderAPI::DEVICE_Lost)
	{
		throw nullptr;
	}
}

void APITestBed::OnResize(unsigned int width, unsigned int height)
{
	//build projection matrix;
	float aspect = width * 1.0f / height;
	const float PI = 3.1415926f;
	const float fov = PI  * 0.25f;
	float nearTop = tanf(fov * 0.5f);
	float nearRight = nearTop * aspect;
	m_matProj[0] = 1.0f / nearRight;
	m_matProj[5] = 1.0f / nearTop;
	m_defaultSwapChain->OnResize(width, height);
}

bool APITestBed::LoadDLL()
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
		return false;
	}

	return true;
}

bool APITestBed::CreateDeviceAndContext(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight)
{
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
	m_defaultSwapChain = m_pDevice->GetDefaultSwapChain();

	desc.hWindow = hWindowEditor;
	desc.backbufferFormat = RenderAPI::BACKBUFFER_XRGB8;
	desc.zbufferFormat = RenderAPI::ZBUFFER_D24S8;
	desc.aaMode = RenderAPI::AA_Disable;
	desc.backbufferWidth = backBufferWidth;
	desc.backbufferHeight = backBufferHeight;
	m_editorSwapChain = m_pDevice->CreateAdditionalSwapChain(desc);

	if (m_defaultSwapChain == nullptr || m_editorSwapChain == nullptr)
	{
		return false;
	}

	return true;
}

struct float3 { float x, y, z; };
struct Vertex
{
	float3 position;
	unsigned int color = 0xFFFF0000;
};

void APITestBed::CreateMesh()
{
	const float kBoxSize = 1.0f;
	float3 vertexPositions[] =
	{
		{ -kBoxSize, -kBoxSize , -kBoxSize },
		{ +kBoxSize, -kBoxSize , -kBoxSize },
		{ +kBoxSize, +kBoxSize , -kBoxSize },
		{ -kBoxSize, +kBoxSize , -kBoxSize },
		{ -kBoxSize, -kBoxSize , +kBoxSize },
		{ +kBoxSize, -kBoxSize , +kBoxSize },
		{ +kBoxSize, +kBoxSize , +kBoxSize },
		{ -kBoxSize, +kBoxSize , +kBoxSize },

	};

	const int kVertexCount = 24;
	const int kIndexCount = 36;
	std::vector<Vertex> vertices(kVertexCount);
	std::vector<unsigned short> indices(kIndexCount);
	int index = 0;
	//front
	vertices[index++].position = vertexPositions[0];
	vertices[index++].position = vertexPositions[1];
	vertices[index++].position = vertexPositions[2];
	vertices[index++].position = vertexPositions[3];

	//back
	vertices[index++].position = vertexPositions[5];
	vertices[index++].position = vertexPositions[4];
	vertices[index++].position = vertexPositions[7];
	vertices[index++].position = vertexPositions[6];

	//left	 
	vertices[index++].position = vertexPositions[4];
	vertices[index++].position = vertexPositions[0];
	vertices[index++].position = vertexPositions[3];
	vertices[index++].position = vertexPositions[7];

	//right
	vertices[index++].position = vertexPositions[1];
	vertices[index++].position = vertexPositions[5];
	vertices[index++].position = vertexPositions[6];
	vertices[index++].position = vertexPositions[2];

	//top
	vertices[index++].position = vertexPositions[3];
	vertices[index++].position = vertexPositions[2];
	vertices[index++].position = vertexPositions[6];
	vertices[index++].position = vertexPositions[7];

	//botton
	vertices[index++].position = vertexPositions[4];
	vertices[index++].position = vertexPositions[5];
	vertices[index++].position = vertexPositions[1];
	vertices[index++].position = vertexPositions[0];

	std::vector<RenderAPI::VertexElement> elements(2);
	elements[0].Format = RenderAPI::INPUT_Float3;
	elements[0].SemanticName = RenderAPI::SEMANTIC_POSITION;

	elements[1].Format = RenderAPI::INPUT_Color4;
	elements[1].SemanticName = RenderAPI::SEMANTIC_COLOR;

	m_pBoxVertexBuffer = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Immuable, 24, sizeof(Vertex), &(elements[0]), elements.size(), &(vertices[0]));

	index = 0;
	for (int i = 0; i < 6; i++)
	{

		indices[index++] = i * 4 + 0;
		indices[index++] = i * 4 + 2;
		indices[index++] = i * 4 + 1;

		indices[index++] = i * 4 + 0;
		indices[index++] = i * 4 + 3;
		indices[index++] = i * 4 + 2;
	}

	m_pBoxIndexBuffer = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_Immuable, RenderAPI::INDEX_Int16, kIndexCount, &(indices[0]));

	m_vertexBufferInfos.resize(1);
	m_vertexBufferInfos[0].BufferPtr = m_pBoxVertexBuffer;
	m_vertexBufferInfos[0].Offset = 0;
	m_vertexBufferInfos[0].Stride = m_pBoxVertexBuffer->GetVertexStride();
}

void APITestBed::CreateMaterial()
{
	m_pEffect = m_pDevice->CreateFXEffectFromFile("TintColor.fx");
	m_pEffect->SetValidateTechnique();
}

void APITestBed::DoEffectDraw()
{
	int passCount = m_pEffect->Begin();
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffect->BeginPass(i))
				continue;
			m_pEffect->SetMatrix("g_matWorld", m_matWorld);
			m_pEffect->SetMatrix("g_matView", m_matView);
			m_pEffect->SetMatrix("g_matProj", m_matProj);
			m_pEffect->CommitChange();
			m_pContext->SetVertexBuffers(0, &(m_vertexBufferInfos[0]), m_vertexBufferInfos.size());
			m_pContext->SetIndexBuffer(m_pBoxIndexBuffer, 0);
			const int faceCount = 6 * 2;
			m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, faceCount);

			m_pEffect->EndPass();
		}
		m_pEffect->End();
	}
}
