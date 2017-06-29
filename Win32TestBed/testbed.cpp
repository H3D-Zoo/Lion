#include "testbed.h"
#include <timeapi.h>
#include <time.h>
#include "Math/gmlcolor.h"
#define DLLName "DX9API.dll"

bool APITestBed::Init(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight)
{
	srand(static_cast<unsigned int>(time(0)));
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
	CreatePartcleMesh();
	CreateMaterial();


	const int kMatrixLength = sizeof(float) * 16;
	m_matWorldBox.identity();
	m_matWorldParticle = gml::mat44::translate(2.5, 0, 0);
	m_matView = gml::mat44::look_at(gml::vec3(0, 0, -20), gml::vec3(0, 0, 0), gml::vec3::up());
	m_matInvView = m_matView.inversed();
	return true;
}

template<class T> void Release(T& pointer)
{
	if (pointer)
	{
		pointer->Release();
		pointer = nullptr;
	}
}

void APITestBed::Deinit()
{
	Release(m_pBoxVertexBuffer);
	Release(m_pBoxIndexBuffer);
	Release(m_pParticleVBS);
	Release(m_pParticleVBD);
	Release(m_pParticleIB);
	Release(m_pEffectTintColor);
	Release(m_pEffectParticle);
	Release(m_editorSwapChain);
	Release(m_defaultSwapChain);
	Release(m_pContext);
	Release(m_pDevice);

	if (m_apiDeinitPtr != nullptr)
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
	float timeScale = timeGetTime()  * 0.001f;
	const float distance = 10;
	static float rho = 0;
	static float phi = 0;
	rho = timeScale * 0.5f;
	phi = sin(timeScale) * gml::PI * 0.25f;
	float sinr = sin(rho);
	float sinp = sin(phi);
	float cosr = cos(rho);
	float cosp = cos(phi);
	gml::vec3 eye;
	eye.x = sinr * cosp;
	eye.z = cosr * cosp;
	eye.y = sinp;

	eye *= distance;

	m_matView = gml::mat44::look_at(eye, gml::vec3(0, 0, 0), gml::vec3::up());
	m_matInvView = m_matView.inversed().transposed();

	auto rt = m_defaultSwapChain->GetRenderTarget();
	auto ds = m_defaultSwapChain->GetDepthStencil();
	m_pContext->SetRenderTarget(0, rt);
	m_pContext->SetDepthStencil(ds);
	m_pContext->SetCullMode(RenderAPI::CULL_CCW);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(rt, 0xFF00FF00);
		m_pContext->ClearDepthStencil(ds, 1.0f, 0);
		DrawBox();
		DrawParticle();
		m_pContext->EndScene();
	}

	m_defaultSwapChain->Present();
	rt->Release();
	ds->Release();

	//sub window
	auto rtEditor = m_editorSwapChain->GetRenderTarget();
	auto dsEditor = m_editorSwapChain->GetDepthStencil();
	m_pContext->SetRenderTarget(0, rtEditor);
	m_pContext->SetDepthStencil(dsEditor);
	m_pContext->SetCullMode(RenderAPI::CULL_CW);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(rtEditor, 0xFFFF0000);
		m_pContext->ClearDepthStencil(dsEditor, 1.0f, 0);
		DrawBox();
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
	unsigned int color = gml::color4::random().rgba();
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

	m_boxVBInfos.resize(1);
	m_boxVBInfos[0].BufferPtr = m_pBoxVertexBuffer;
	m_boxVBInfos[0].Offset = 0;
	m_boxVBInfos[0].Stride = m_pBoxVertexBuffer->GetVertexStride();
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

	std::vector<ParticleVertexS> expandData(kParticleVertexCount);
	std::vector<unsigned short> particleIndices(kParticleIndexCount);
	for (int i = 0; i < kParticleCount; i++)
	{
		int vIndex = i * 4;
		int fIndex = i * 6;
		ParticleVertexS& v0 = expandData[vIndex];
		ParticleVertexS& v1 = expandData[vIndex + 1];
		ParticleVertexS& v2 = expandData[vIndex + 2];
		ParticleVertexS& v3 = expandData[vIndex + 3];
		unsigned short* face0 = &(particleIndices[fIndex]);
		unsigned short* face1 = &(particleIndices[fIndex + 3]);

		v0.offset.x = -kParticleSize;
		v0.offset.y = -kParticleSize;
		v0.offset.z = 0;
		v0.texcoord.x = 0;
		v0.texcoord.y = 0;

		v1.offset.x = +kParticleSize;
		v1.offset.y = -kParticleSize;
		v1.offset.z = 0;
		v1.texcoord.x = 1;
		v1.texcoord.y = 0;

		v2.offset.x = +kParticleSize;
		v2.offset.y = +kParticleSize;
		v2.offset.z = 0;
		v2.texcoord.x = 1;
		v2.texcoord.y = 1;

		v3.offset.x = -kParticleSize;
		v3.offset.y = +kParticleSize;
		v3.offset.z = 0;
		v3.texcoord.x = 0;
		v3.texcoord.y = 1;

		face0[0] = vIndex + 0;
		face0[1] = vIndex + 2;
		face0[2] = vIndex + 1;

		face1[0] = vIndex + 0;
		face1[1] = vIndex + 3;
		face1[2] = vIndex + 2;
	}

	std::vector<RenderAPI::VertexElement> elements(2);
	elements[0].Format = RenderAPI::INPUT_Float3;
	elements[0].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;
	elements[0].SemanticIndex = 0;

	elements[1].Format = RenderAPI::INPUT_Float2;
	elements[1].SemanticName = RenderAPI::SEMANTIC_TEXCOORD;
	elements[1].SemanticIndex = 1;

	m_pParticleVBS = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Immuable, kParticleVertexCount, sizeof(ParticleVertexS), &(elements[0]), 2, &(expandData[0]));
	m_pParticleIB = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_Immuable, RenderAPI::INDEX_Int16, kParticleIndexCount, &(particleIndices[0]));

	elements[0].Format = RenderAPI::INPUT_Float3;
	elements[0].SemanticName = RenderAPI::SEMANTIC_POSITION;
	elements[0].SemanticIndex = 0;
	m_pParticleVBD = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Dynamic, kParticleVertexCount, sizeof(gml::vec3), &(elements[0]), 1, nullptr);
	FillDynamicParticleVBWithRandomData();

	m_particleVBInfos.resize(2);
	m_particleVBInfos[0].BufferPtr = m_pParticleVBD;
	m_particleVBInfos[0].Stride = m_pParticleVBD->GetVertexStride();

	m_particleVBInfos[1].BufferPtr = m_pParticleVBS;
	m_particleVBInfos[1].Stride = m_pParticleVBS->GetVertexStride();
}

void APITestBed::CreateMaterial()
{
	m_pEffectTintColor = m_pDevice->CreateFXEffectFromFile("TintColor.fx");
	m_pEffectTintColor->SetValidateTechnique();

	m_pEffectParticle = m_pDevice->CreateFXEffectFromFile("Particle.fx");
	m_pEffectParticle->SetValidateTechnique();
}

float RandomRangeF(float  min, float max) { return min + rand() * (max - min) / RAND_MAX; }

void APITestBed::FillDynamicParticleVBWithRandomData()
{
	const float kParticleSize = 0.1f;
	const float range = 1.5f;
	gml::vec3* vertices = (gml::vec3*)m_pParticleVBD->DiscardLock();
	if (vertices != nullptr)
	{
		for (int i = 0; i < kParticleCount; i++)
		{
			gml::vec3& v0 = vertices[i * 4];
			gml::vec3& v1 = vertices[i * 4 + 1];
			gml::vec3& v2 = vertices[i * 4 + 2];
			gml::vec3& v3 = vertices[i * 4 + 3];

			v0.x = RandomRangeF(-range, range);
			v0.y = RandomRangeF(-range, range);
			v0.z = RandomRangeF(-range, range);
			v1 = v2 = v3 = v0;
		}

		m_pParticleVBD->Unlock();
	}
}

void APITestBed::DrawBox()
{
	int passCount = m_pEffectTintColor->Begin();
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffectTintColor->BeginPass(i))
				continue;
			m_pEffectTintColor->SetMatrix("g_matWorld", (float*)m_matWorldBox.m);
			m_pEffectTintColor->SetMatrix("g_matView", (float*)m_matView.m);
			m_pEffectTintColor->SetMatrix("g_matProj", (float*)m_matProj.m);
			m_pEffectTintColor->CommitChange();
			m_pContext->SetVertexBuffers(0, &(m_boxVBInfos[0]), m_boxVBInfos.size());
			m_pContext->SetIndexBuffer(m_pBoxIndexBuffer, 0);
			const int faceCount = 6 * 2;
			m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, faceCount);

			m_pEffectTintColor->EndPass();
		}
		m_pEffectTintColor->End();
	}
}

void APITestBed::DrawParticle()
{
	int passCount = m_pEffectParticle->Begin();
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffectParticle->BeginPass(i))
				continue;
			m_pEffectParticle->SetMatrix("g_matWorld", (float*)m_matWorldParticle.m);
			m_pEffectParticle->SetMatrix("g_matView", (float*)m_matView.m);
			m_pEffectParticle->SetMatrix("g_matProj", (float*)m_matProj.m);
			m_pEffectParticle->SetValue("g_cameraX", (float*)m_matInvView.row[0], sizeof(gml::vec4));
			m_pEffectParticle->SetValue("g_cameraY", (float*)m_matInvView.row[1], sizeof(gml::vec4));
			m_pEffectParticle->CommitChange();
			m_pContext->SetVertexBuffers(0, &(m_particleVBInfos[0]), m_particleVBInfos.size());
			m_pContext->SetIndexBuffer(m_pParticleIB, 0);
			m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, kParticleFaceCount);

			m_pEffectParticle->EndPass();
		}
		m_pEffectParticle->End();
	}
}
