#include "testbed.h"

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
	CreatePartcleMesh();

	const int kMatrixLength = sizeof(float) * 16;
	m_matWorld.identity();
	m_matView.identity();
	m_matProj.identity();

	m_matView = gml::mat44::look_at(gml::vec3(5, 10, -20), gml::vec3::zero(), gml::vec3::up());
	return true;
}

void APITestBed::Deinit()
{
	if (m_pBoxVertexBuffer)
	{
		m_pBoxVertexBuffer->Release();
		m_pBoxVertexBuffer = nullptr;
	}

	if (m_pBoxIndexBuffer)
	{
		m_pBoxIndexBuffer->Release();
		m_pBoxIndexBuffer = nullptr;
	}

	if (m_pParticleVBS)
	{
		m_pParticleVBS->Release();
		m_pParticleVBS = nullptr;
	}

	if (m_pParticleVBD)
	{
		m_pParticleVBD->Release();
		m_pParticleVBD = nullptr;
	}

	if (m_pParticleIB)
	{
		m_pParticleIB->Release();
		m_pParticleIB = nullptr;
	}

	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
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
	m_pContext->SetCullMode(RenderAPI::CULL_CW);
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
	m_matProj = gml::mat44::perspective_lh(gml::degree(45), width*1.0f / height, 1.0f, 100.0f);
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

struct BoxVertex
{
	gml::vec3 position;
	unsigned int color = 0xFFFF0000;
};

void APITestBed::CreateMesh()
{
	const float kBoxSize = 1.0f;
	gml::vec3 vertexPositions[] =
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
	std::vector<BoxVertex> vertices(kVertexCount);
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

	m_pBoxVertexBuffer = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Immuable, 24, sizeof(BoxVertex), &(elements[0]), elements.size(), &(vertices[0]));

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

struct ParticleVertexS
{
	gml::vec3 offset;
	gml::vec2 texcoord;
};

void APITestBed::CreatePartcleMesh()
{
	const float kParticleSize = 0.1f;
	const int kParticleVertexCount = kParticleCount * 4;
	const int kIndexCount = kParticleCount * 3 * 2;
	std::vector<ParticleVertexS> expandData(kParticleVertexCount);
	std::vector<unsigned int> particleIndices(kIndexCount);
	for (int i = 0; i < kParticleCount; i++)
	{
		ParticleVertexS& v0 = expandData[i * 4];
		ParticleVertexS& v1 = expandData[i * 4 + 1];
		ParticleVertexS& v2 = expandData[i * 4 + 2];
		ParticleVertexS& v3 = expandData[i * 4 + 3];
		unsigned int* face0 = &(particleIndices[i * 6]);
		unsigned int* face1 = &(particleIndices[i * 6 + 3]);

		v0.offset.x = -kParticleSize;
		v0.offset.y = -kParticleSize;
		v0.offset.z = 0;
		v0.offset.x = 0;
		v0.offset.y = 0;

		v1.offset.x = +kParticleSize;
		v1.offset.y = -kParticleSize;
		v1.offset.z = 0;
		v1.offset.x = 1;
		v1.offset.y = 0;

		v1.offset.x = +kParticleSize;
		v1.offset.y = +kParticleSize;
		v1.offset.z = 0;
		v1.offset.x = 1;
		v1.offset.y = 1;

		v3.offset.x = -kParticleSize;
		v3.offset.y = +kParticleSize;
		v3.offset.z = 0;
		v3.offset.x = 0;
		v3.offset.y = 1;

		face0[0] = 0;
		face0[1] = 2;
		face0[2] = 1;

		face1[0] = 0;
		face1[1] = 3;
		face1[2] = 2;

		face0[0] = 0;
	}

	std::vector<RenderAPI::VertexElement> elements(2);
	elements[0].Format = RenderAPI::INPUT_Float3;
	elements[0].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;
	elements[1].SemanticIndex = 0;

	elements[1].Format = RenderAPI::INPUT_Float2;
	elements[1].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;
	elements[1].SemanticIndex = 1;

	m_pParticleVBS = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Immuable, expandData.size(), sizeof(ParticleVertexS), &(elements[0]), elements.size(), &(expandData[0]));
	m_pParticleIB = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_Immuable, RenderAPI::INDEX_Int16, kIndexCount, &(particleIndices[0]));


	elements[0].Format = RenderAPI::INPUT_Float3;
	elements[0].SemanticName = RenderAPI::SEMANTIC_POSITION;
	elements[0].SemanticIndex = 0;
	m_pParticleVBD = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Dynamic, expandData.size(), sizeof(gml::vec3), &(elements[0]), 1, nullptr);
	FillDynamicParticleVBWithRandomData();
}

void APITestBed::CreateMaterial()
{
	m_pEffect = m_pDevice->CreateFXEffectFromFile("TintColor.fx");
	m_pEffect->SetValidateTechnique();
}

void APITestBed::FillDynamicParticleVBWithRandomData()
{
	gml::vec3* vertices = (gml::vec3*)m_pParticleVBD->DiscardLock();
	if (vertices != nullptr)
	{
		for (int i = 0; i < kParticleCount; i++)
		{
			gml::vec3& v0 = vertices[i * 4];
			gml::vec3& v1 = vertices[i * 4 + 1];
			gml::vec3& v2 = vertices[i * 4 + 2];
			gml::vec3& v3 = vertices[i * 4 + 3];
		}

		m_pParticleVBD->Unlock();
	}
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
			m_pEffect->SetMatrix("g_matWorld", (float*)m_matWorld.m);
			m_pEffect->SetMatrix("g_matView", (float*)m_matView.m);
			m_pEffect->SetMatrix("g_matProj", (float*)m_matProj.m);
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
