#include "testbed.h"
#include <timeapi.h>
#include <time.h>
#include "math/gmlcolor.h"
#include "image/file_data.h"
#include "image/img_data.h"
#include "box.hpp"

#ifdef _DEBUG
#define DLLName "DX9API_d.dll"
#else
#define DLLName "DX9API.dll"
#endif

bool APITestBed::Init(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight)
{
	srand(static_cast<unsigned int>(time(0)));
	if (!LoadDLL())
	{
		return false;
	}

	m_pAPIInstance = m_createAPIInstance();
	if (m_pAPIInstance == nullptr)
	{
		return false;
	}

	if (!CreateDeviceAndContext(hWindow, hWindowEditor, backBufferWidth, backBufferHeight))
	{
		return false;
	}

	CreateMesh();
	CreatePartcleMesh();
	CreateQuadMesh();
	CreateMaterial();

	m_textureSize.x = 1024;
	m_textureSize.y = 1024;
	m_pRenderTexture = m_pDevice->CreateRenderTarget(RenderAPI::TEX_XRGB, 1024, 1024);
	m_pRenderDepth = m_pDevice->CreateDepthStencil(RenderAPI::DS_D24S8, 1024, 1024);

	const int kMatrixLength = sizeof(float) * 16;
	m_matWorldBoxs.resize(100);

	gml::mat44 matScale = gml::mat44::scale(0.25f);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			m_matWorldBoxs[i * 10 + j] = gml::mat44::translate(-4.5f + j, 0, -4.5f + i) * matScale;
	}

	m_matWorldParticle = gml::mat44::translate(2.5f, 0, 0);
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
	Release(m_pParticleTexture);
	Release(m_pRenderTexture);
	Release(m_pRenderDepth);
	Release(m_pEffectTintColor);
	Release(m_pEffectParticle);
	Release(m_pEffectSimpleTexture);
	Release(m_editorSwapChain);
	Release(m_defaultSwapChain);
	Release(m_pContext);
	Release(m_pDevice);
	Release(m_pAPIInstance);
	if (m_hRenderAPIDLL != nullptr)
	{
		FreeLibrary(m_hRenderAPIDLL);
		m_hRenderAPIDLL = nullptr;
	}
}

void APITestBed::Update()
{
	static unsigned int lastStamp = timeGetTime();
	static unsigned int accStamp = 0;
	m_particleInstance.Update(timeGetTime() - lastStamp);
	accStamp += timeGetTime() - lastStamp;
	lastStamp = timeGetTime();

	float timeScale = accStamp * 0.001f;
	const float distance = 10;
	static float rho = 0;
	static float phi = 0;
	rho = timeScale * 0.15f;
	phi = sin(timeScale) * gml::PI * 0.2f;
	float sinr = sin(rho);
	float sinp = sin(phi);
	float cosr = cos(rho);
	float cosp = cos(phi);
	gml::vec3 eye;
	eye.x = sinr * cosp;
	eye.z = cosr * cosp;
	eye.y = sinp;
	//eye.normalize();
	eye *= distance;

	m_matView = gml::mat44::look_at(eye, gml::vec3(0, 0, 0), gml::vec3::up());
	m_matInvView = m_matView.inversed().transposed();

	m_pContext->SetCullMode(RenderAPI::CULL_CCW);
	m_pContext->SetRenderTarget(0, m_pRenderTexture);
	m_pContext->SetDepthStencil(m_pRenderDepth);
	RenderAPI::Viewport viewport;
	viewport.Width = 1024;
	viewport.Height = 1024;
	m_pContext->SetViewport(viewport);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(0xFF000099);
		m_pContext->ClearDepthBuffer(1.0f);
		DrawParticle(m_matProjRTT);
		m_pContext->EndScene();
	}

	m_pContext->SetCullMode(RenderAPI::CULL_CCW);
	auto ds = m_defaultSwapChain->GetDepthStencil();
	auto rt = m_defaultSwapChain->GetRenderTarget();
	m_pContext->SetRenderTarget(0, rt);
	m_pContext->SetDepthStencil(ds);
	ds->Release();
	rt->Release();
	m_pContext->SetViewport(m_bbViewport);
	m_pContext->ClearRenderTarget(0xFF009999);
	RenderAPI::ScissorState scissorState;
	scissorState.IsEnable = true;
	scissorState.Left = 100;
	scissorState.Top = 50;
	scissorState.Right = 1300;
	scissorState.Bottom = 550;
	m_pContext->SetScissorState(scissorState);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(0xFF009900);
		m_pContext->ClearDepthBuffer(1.0f);
		DrawBox(RenderAPI::TextureAddress::TEX_ADDRESS_Repeat, false);
		DrawParticle(m_matProj);
		DrawRTTQuad();
		m_pContext->EndScene();
	}
	scissorState.IsEnable = false;
	m_pContext->SetScissorState(scissorState);
	m_defaultSwapChain->Present();

	//sub window
	auto rtEditor = m_editorSwapChain->GetRenderTarget();
	auto dsEditor = m_editorSwapChain->GetDepthStencil();
	m_pContext->SetRenderTarget(0, rtEditor);
	m_pContext->SetDepthStencil(dsEditor);
	rtEditor->Release();
	dsEditor->Release();

	m_pContext->SetCullMode(RenderAPI::CULL_CW);
	if (m_pContext->BeginScene())
	{
		m_pContext->ClearRenderTarget(0xFF990000);
		m_pContext->ClearDepthBuffer(1.0f);
		DrawBox(RenderAPI::TextureAddress::TEX_ADDRESS_Clamp, true);
		m_pContext->EndScene();
	}
	m_editorSwapChain->Present();
	if (m_pContext->GetState() == RenderAPI::DEVICE_Lost)
	{
		throw nullptr;
	}
}

void APITestBed::OnResize(unsigned int width, unsigned int height)
{
	//build projection matrix;
	m_matProj = gml::mat44::perspective_lh(gml::degree(45), width*1.0f / height, 1.0f, 100.0f);
	m_matProjRTT = gml::mat44::perspective_lh(gml::degree(45), 1.0f, 1.0f, 100.0f);

	// depend of data in quadVertices
	m_texelOffsetOffScreen.x = 1.0f / (width * 0.5f * 0.75f);
	m_texelOffsetOffScreen.y = 1.0f / (height * 0.5f * 0.75f);
	m_defaultSwapChain->OnResize(width, height);
	m_bbViewport.Width = width;
	m_bbViewport.Height = height;
}

bool APITestBed::LoadDLL()
{
	m_hRenderAPIDLL = LoadLibraryA(DLLName);
	if (m_hRenderAPIDLL == nullptr)
	{
		return false;
	}

	m_createAPIInstance = (CreateAPIGlobal)GetProcAddress(m_hRenderAPIDLL, "CreateAPIInstance");

	if (m_createAPIInstance == nullptr)
	{
		return false;
	}

	return true;
}

bool APITestBed::CreateDeviceAndContext(HWND hWindow, HWND hWindowEditor, unsigned int backBufferWidth, unsigned int backBufferHeight)
{
	RenderAPI::SwapChainDesc desc;
	desc.hWindow = hWindow;
	desc.backbufferFormat = RenderAPI::RT_XRGB8;
	desc.zbufferFormat = RenderAPI::DS_D24S8;
	desc.aaMode = RenderAPI::AA_Disable;
	desc.backbufferWidth = backBufferWidth;
	desc.backbufferHeight = backBufferHeight;
	auto rst = m_pAPIInstance->CreateDeviceAndContext(desc, false, false);

	if (!rst.Success)
	{
		return false;
	}
	m_pDevice = rst.DevicePtr;
	m_pContext = rst.ContextPtr;
	m_defaultSwapChain = m_pDevice->GetDefaultSwapChain();

	desc.hWindow = hWindowEditor;
	desc.backbufferFormat = RenderAPI::RT_XRGB8;
	desc.zbufferFormat = RenderAPI::DS_D24S8;
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

void APITestBed::CreateMesh()
{
	BoxMesh boxMesh;
	auto pBoxVertexBuffer = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_StaticWO, boxMesh.GetVertexCount(), sizeof(BoxVertex));
	char* pBoxVB = (char*)pBoxVertexBuffer->LockAll(RenderAPI::LOCK_Discard);
	if (pBoxVB != nullptr)
	{
		memcpy(pBoxVB, boxMesh.GetVerticesPtr(), pBoxVertexBuffer->GetLength());
		pBoxVertexBuffer->Unlock();
	}

	Box.index = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_StaticWOManaged, RenderAPI::INDEX_Int16, boxMesh.GetIndexCount());
	char* pBoxIB = (char*)Box.index->LockAll(RenderAPI::LOCK_NoOverWrite);
	if (pBoxIB != nullptr)
	{
		memcpy(pBoxIB, boxMesh.GetIndicesPtr(), Box.index->GetLength());
		Box.index->Unlock();
	}

	Box.declaration = m_pDevice->CreateVertexDeclaration(boxMesh.GetElementsPtr(), boxMesh.GetElementCount());
	Box.vertex.resize(1);
	Box.vertex[0].BufferPtr = pBoxVertexBuffer;
	Box.vertex[0].Offset = 0;
}

void APITestBed::CreateQuadMesh()
{
	struct QuadVertex
	{
		gml::vec3 position;
		gml::vec2 texcoord;
	} quadVertices[4] =
	{
		{ { 0.25, 0.25, 0 }, { 0, 1} },
		{ { 1.00, 0.25, 0 }, { 1, 1 } },
		{ { 1.00, 1.00, 0 }, { 1, 0 } },
		{ { 0.25, 1.00, 0 }, { 0, 0 } },
	};

	RenderAPI::VertexElement elements[2];
	elements[0].SemanticName = RenderAPI::SEMANTIC_Position;
	elements[1].SemanticName = RenderAPI::SEMANTIC_Texcoord;
	elements[0].Format = RenderAPI::INPUT_Float3;
	elements[1].Format = RenderAPI::INPUT_Float2;
	elements[0].SemanticIndex = 0;
	elements[1].SemanticIndex = 0;

	unsigned short quadIndices[] = { 0, 2, 1, 0, 3, 2 };


	auto pQuadVB = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_StaticWOManaged, 4, sizeof(QuadVertex));
	char* pQuadVBPtr = (char*)pQuadVB->LockAll(RenderAPI::LOCK_Normal);
	if (pQuadVBPtr != nullptr)
	{
		memcpy(pQuadVBPtr, quadVertices, pQuadVB->GetLength());
		pQuadVB->Unlock();
	}

	Quad.index = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_StaticWOManaged, RenderAPI::INDEX_Int16, 6);
	char* pQuadIB = (char*)Quad.index->LockAll(RenderAPI::LOCK_Normal);
	if (pQuadIB != nullptr)
	{
		memcpy(pQuadIB, quadIndices, Quad.index->GetLength());
		Quad.index->Unlock();
	}

	Quad.declaration = m_pDevice->CreateVertexDeclaration(elements, 2);
	Quad.vertex.resize(1);
	Quad.vertex[0].BufferPtr = pQuadVB;
	Quad.vertex[0].Offset = 0;
}

void APITestBed::CreatePartcleMesh()
{
	auto pParticleVBS = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_StaticWOManaged, m_particleInstance.kParticleVertexCount, sizeof(ParticleVertexS));
	char* pParticleVBSPtr = (char*)pParticleVBS->LockAll(RenderAPI::LOCK_Normal);
	if (pParticleVBSPtr != nullptr)
	{
		memcpy(pParticleVBSPtr, m_particleInstance.GetSVerticesPtr(), pParticleVBS->GetLength());
		pParticleVBS->Unlock();
	}

	auto pParticleVBD = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Dynamic, m_particleInstance.kParticleVertexCount, sizeof(ParticleVertexD));

	Particle.index = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_StaticWOManaged, RenderAPI::INDEX_Int16, m_particleInstance.kParticleIndexCount);
	char* pParticleIBSPtr = (char*)Particle.index->LockAll(RenderAPI::LOCK_Normal);
	if (pParticleIBSPtr != nullptr)
	{
		memcpy(pParticleIBSPtr, m_particleInstance.GetIndicesPtr(), Particle.index->GetLength());
		Particle.index->Unlock();
	}

	std::vector<RenderAPI::VertexElement> elements;
	for (unsigned int i = 0; i < m_particleInstance.GetSElementCount(); i++)
	{
		elements.push_back(m_particleInstance.GetSElementsPtr()[i]);
	}
	for (unsigned int i = 0; i < m_particleInstance.GetDElementCount(); i++)
	{
		elements.push_back(m_particleInstance.GetDElementsPtr()[i]);
		elements.back().StreamIndex = 1;
	}

	Particle.declaration = m_pDevice->CreateVertexDeclaration(&(elements[0]), elements.size());
	Particle.vertex.resize(2);
	Particle.vertex[0].BufferPtr = pParticleVBS;
	Particle.vertex[1].BufferPtr = pParticleVBD;
	Particle.vertex[0].Offset = 0;
	Particle.vertex[1].Offset = 0;
}

void APITestBed::CreateMaterial()
{
	m_pEffectTintColor = m_pDevice->CreateFXEffectFromFile("../../Win32TestBed/TintColor.fx", "");
	m_pEffectTintColor->SetValidateTechnique();

	m_pEffectParticle = m_pDevice->CreateFXEffectFromFile("../../Win32TestBed/Particle.fx", "");
	m_pEffectParticle->SetValidateTechnique();
	m_hParamParticleTex = m_pEffectParticle->GetParameterByName("g_particleTexture");

	m_pEffectSimpleTexture = m_pDevice->CreateFXEffectFromFile("../../Win32TestBed/SimpleTexture.fx", "");
	m_hTechStencil = m_pEffectSimpleTexture->GetTechniqueByName("SimpleTextureStencil");
	m_hTechSimple = m_pEffectSimpleTexture->GetTechniqueByName("SimpleTexture");
	m_hParamTexture = m_pEffectSimpleTexture->GetParameterByName("g_texture");



	file_data pngFileData = read_file("../../Win32TestBed/particle.png");
	if (pngFileData.is_valid())
	{
		img_data png = read_image(pngFileData.buffer);

		if (png.is_valid())
		{
			RenderAPI::TextureFormat texformat;

			if (png.format == color_format::bgra32)
			{
				texformat = RenderAPI::TEX_ARGB;
			}
			else if (png.format == color_format::bgr24)
			{
				texformat = RenderAPI::TEX_XRGB;
			}

			m_pParticleTexture = m_pDevice->CreateTexture2D(RenderAPI::RESUSAGE_StaticWOManaged, texformat, png.width, png.height, 0, true);
			RenderAPI::MappedResource mres = m_pParticleTexture->LockRect(0, RenderAPI::LOCK_Normal);
			if (mres.Success)
			{
				unsigned int linePitch = mres.LinePitch < png.line_pitch ? mres.LinePitch : png.line_pitch;
				for (unsigned int i = 0; i < png.height; i++)
				{
					char* dstPtr = (char*)mres.DataPtr + mres.LinePitch * i;
					char* srcPtr = (char*)png.buffer + png.line_pitch* i;
					memcpy(dstPtr, srcPtr, linePitch);
				}
				m_pParticleTexture->UnlockRect(0);
			}


			png.destroy();
		}
		pngFileData.destroy();
	}

	file_data lenaFile = read_file("../../Win32TestBed/lena.bmp");
	if (lenaFile.is_valid())
	{
		img_data bmp = read_image(lenaFile.buffer);

		if (bmp.is_valid())
		{
			RenderAPI::TextureFormat texformat;

			if (bmp.format == color_format::bgra32)
			{
				texformat = RenderAPI::TEX_ARGB;
			}
			else if (bmp.format == color_format::bgr24)
			{
				texformat = RenderAPI::TEX_XRGB;
			}

			m_pBoxTexture = m_pDevice->CreateTexture2D(RenderAPI::RESUSAGE_StaticWOManaged, texformat, bmp.width, bmp.height, 0, true);

			RenderAPI::MappedResource res = m_pBoxTexture->LockRect(0, RenderAPI::LOCK_Discard);

			if (res.Success)
			{
				char* dstPtr = (char*)res.DataPtr;
				char* srcPtr = (char*)bmp.buffer;
				for (unsigned int i = 0; i < bmp.height; i++)
				{
					for (unsigned int j = 0; j < bmp.width; j++)
					{
						char* dstColor = dstPtr + i * res.LinePitch + j * 4;
						char* srcColor = srcPtr + i * bmp.line_pitch + j * 3;
						memcpy(dstColor, srcColor, 3);
					}
				}
				m_pBoxTexture->UnlockRect(0);
				m_pBoxTexture->GenerateMipmaps();
			}
			bmp.destroy();
		}
		lenaFile.destroy();
	}

}

float RandomRangeF(float  min, float max) { return min + rand() * (max - min) / RAND_MAX; }

void APITestBed::DrawBox(RenderAPI::TextureAddress address, bool alphaBlending)
{
	RenderAPI::DepthTestingState depthState;
	depthState.IsEnable = true;
	RenderAPI::TextureSampler sampler;
	sampler.AddressU = sampler.AddressV = address;
	sampler.MinFilter = RenderAPI::FILTER_Linear;
	sampler.MagFilter = RenderAPI::FILTER_Linear;
	sampler.MipFilter = RenderAPI::FILTER_Linear;

	int passCount = m_pEffectTintColor->Begin(false);
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffectTintColor->BeginPass(i))
				continue;

			Box.Set(m_pContext);
			m_pContext->SetTexture(0, m_pBoxTexture);
			m_pContext->SetTextureSampler(0, sampler);
			m_pEffectTintColor->SetMatrixTranspose("g_matView", (float*)m_matView.m);
			m_pEffectTintColor->SetMatrixTranspose("g_matProj", (float*)m_matProj.m);
			if (alphaBlending)
			{
				RenderAPI::BlendState abstate;
				abstate.IsEnable = true;
				abstate.SrcBlend = RenderAPI::BLEND_One;
				abstate.DstBlend = RenderAPI::BLEND_InvSrcColor;
				abstate.BlendOp = RenderAPI::BLENDOP_Sub;
				m_pContext->SetBlendState(abstate);
			}
			else
			{
				RenderAPI::BlendState abstate;
				m_pContext->SetBlendState(abstate);
			}

			const int faceCount = 6 * 2;
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if ((i % 2) ^ (j % 2))
					{
						depthState.Function = RenderAPI::COMPARE_Less;
						m_pContext->SetDepthTestingState(depthState);
					}
					else
					{

						depthState.Function = RenderAPI::COMPARE_Greater;
						m_pContext->SetDepthTestingState(depthState);
					}
					m_pEffectTintColor->SetMatrixTranspose("g_matWorld", (float*)m_matWorldBoxs[i * 10 + j].m);
					m_pEffectTintColor->CommitChange();
					m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, 0, faceCount);
				}
			}
			m_pEffectTintColor->EndPass();
		}
		m_pEffectTintColor->End();
	}

	if (alphaBlending)
	{
		RenderAPI::BlendState abstate;
		abstate.IsEnable = true;
		m_pContext->SetBlendState(abstate);
	}

	depthState.Function = RenderAPI::COMPARE_Less;
	m_pContext->SetDepthTestingState(depthState);
}

void APITestBed::DrawParticle(const gml::mat44& matProj)
{
	RenderAPI::AlphaTestingState alphaState;

	int passCount = m_pEffectParticle->Begin(false);
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffectParticle->BeginPass(i))
				continue;
			m_pEffectParticle->SetMatrixTranspose("g_matWorld", (float*)m_matWorldParticle.m);
			m_pEffectParticle->SetMatrixTranspose("g_matView", (float*)m_matView.m);
			m_pEffectParticle->SetMatrixTranspose("g_matProj", (float*)matProj.m);
			m_pEffectParticle->SetValue("g_cameraX", (float*)m_matInvView.row[0], sizeof(gml::vec4));
			m_pEffectParticle->SetValue("g_cameraY", (float*)m_matInvView.row[1], sizeof(gml::vec4));
			m_pEffectParticle->SetTexture(m_hParamParticleTex, m_pParticleTexture);
			Particle.Set(m_pContext);
			m_pEffectParticle->CommitChange();
			UploadParticlesAndCommitDrawcalls();

			m_pEffectParticle->EndPass();
		}
		m_pEffectParticle->End();
	}
}

void APITestBed::DrawRTTQuad()
{
	m_pContext->ClearStencilBuffer(0);

	int numQuadVB = Quad.vertex[0].BufferPtr->GetVertexCount();

	RenderAPI::AlphaTestingState alphaState;
	alphaState.IsEnable = true;
	alphaState.Reference = 128;
	m_pContext->SetAlphaTestingState(alphaState);

	m_pEffectSimpleTexture->SetTechnique(m_hTechStencil);
	int passCount = m_pEffectSimpleTexture->Begin(false);
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffectSimpleTexture->BeginPass(i))
				continue;
			m_pEffectSimpleTexture->SetValue("g_texelOffset", &(m_texelOffsetOffScreen[0]), sizeof(gml::vec4));
			RenderAPI::Texture2D* pTexture = m_pRenderTexture->GetTexturePtr();
			m_pEffectSimpleTexture->SetTexture(m_hParamTexture, pTexture);
			pTexture->Release();
			m_pEffectSimpleTexture->CommitChange();
			Quad.Set(m_pContext);
			m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, 0, 2);
			m_pEffectSimpleTexture->EndPass();
		}
		m_pEffectSimpleTexture->End();
	}
	alphaState.IsEnable = false;
	m_pContext->SetAlphaTestingState(alphaState);

	m_pEffectSimpleTexture->SetTechnique(m_hTechSimple);
	passCount = m_pEffectSimpleTexture->Begin(false);
	if (passCount > 0)
	{
		for (int i = 0; i < passCount; i++)
		{
			if (!m_pEffectSimpleTexture->BeginPass(i))
				continue;
			RenderAPI::Texture2D* pTexture = m_pRenderTexture->GetTexturePtr();
			m_pEffectSimpleTexture->SetTexture(m_hParamTexture, pTexture);
			pTexture->Release();
			m_pEffectSimpleTexture->CommitChange();
			Quad.Set(m_pContext);
			m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, 0, 2);
			m_pEffectSimpleTexture->EndPass();

		}
		m_pEffectSimpleTexture->End();
	}

	RenderAPI::StencilTestingState stencilState;
	m_pContext->SetStencilTestingState(stencilState);
}

void APITestBed::UploadParticlesAndCommitDrawcalls()
{
	int filledCount = 0;
	int particleCount = m_particleInstance.GetParticleCount();
	while (filledCount < particleCount)
	{
		const float kParticleSize = 0.1f;
		const float range = 1.5f;
		auto dvb = Particle.vertex[1].BufferPtr;
		ParticleVertexD* vertices = (ParticleVertexD*)dvb->LockAll(RenderAPI::LOCK_Discard);
		if (vertices != nullptr)
		{
			filledCount += m_particleInstance.FillVertexBuffer(vertices, filledCount);
			dvb->Unlock();
		}

		m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, filledCount * 4, 0, filledCount * 2);
	}

}

void APITestBed::Mesh::Set(RenderAPI::Context* pContext)
{
	pContext->SetVertexBuffers(&(vertex[0]), vertex.size());
	pContext->SetIndexBuffer(index, 0);
	pContext->SetVertexDeclaration(declaration);
}

void APITestBed::Mesh::Release()
{

	auto curr = vertex.begin();
	auto end = vertex.end();
	while (curr != end)
	{
		::Release(curr->BufferPtr);
		curr++;
	}
	vertex.clear();
	::Release(index);
	::Release(declaration);
}
