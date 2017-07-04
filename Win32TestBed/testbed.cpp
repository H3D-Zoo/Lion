#include "testbed.h"
#include <timeapi.h>
#include <time.h>
#include "math/gmlcolor.h"
#include "image/file_data.h"
#include "image/img_data.h"
#include "box.hpp"
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
	Release(m_pParticleTexture);
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
	static unsigned int lastStamp = timeGetTime();
	m_particleInstance.Update(timeGetTime() - lastStamp);
	lastStamp = timeGetTime();

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
		DrawBox(RenderAPI::TextureAddress::TEX_ADDRESS_Repeat);
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
		DrawBox(RenderAPI::TextureAddress::TEX_ADDRESS_Clamp);
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


void APITestBed::CreateMesh()
{
	BoxMesh boxMesh;
	m_pBoxVertexBuffer = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Default, boxMesh.GetVertexCount(), sizeof(BoxVertex), boxMesh.GetElementsPtr(), boxMesh.GetElementCount(), boxMesh.GetVerticesPtr());
	m_pBoxIndexBuffer = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_Immuable, RenderAPI::INDEX_Int16, boxMesh.GetIndexCount(), boxMesh.GetIndicesPtr());

	m_boxVBInfos.resize(1);
	m_boxVBInfos[0].BufferPtr = m_pBoxVertexBuffer;
	m_boxVBInfos[0].Offset = 0;
	m_boxVBInfos[0].Stride = m_pBoxVertexBuffer->GetVertexStride();
}

void APITestBed::CreatePartcleMesh()
{
	m_pParticleVBS = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Immuable, m_particleInstance.kParticleVertexCount, sizeof(ParticleVertexS), m_particleInstance.GetSElementsPtr(), m_particleInstance.GetSElementCount(), m_particleInstance.GetSVerticesPtr());
	m_pParticleVBD = m_pDevice->CreateVertexBuffer(RenderAPI::RESUSAGE_Dynamic, m_particleInstance.kParticleVertexCount, sizeof(ParticleVertexD), m_particleInstance.GetDElementsPtr(), m_particleInstance.GetDElementCount(), nullptr);
	m_pParticleIB = m_pDevice->CreateIndexBuffer(RenderAPI::RESUSAGE_Immuable, RenderAPI::INDEX_Int16, m_particleInstance.kParticleIndexCount, m_particleInstance.GetIndicesPtr());

	m_particleVBInfos.resize(2);
	m_particleVBInfos[0].BufferPtr = m_pParticleVBD;
	m_particleVBInfos[0].Stride = m_pParticleVBD->GetVertexStride();

	m_particleVBInfos[1].BufferPtr = m_pParticleVBS;
	m_particleVBInfos[1].Stride = m_pParticleVBS->GetVertexStride();
}

void APITestBed::CreateMaterial()
{
	m_pEffectTintColor = m_pDevice->CreateFXEffectFromFile("../../Win32TestBed/TintColor.fx");
	m_pEffectTintColor->SetValidateTechnique();

	m_pEffectParticle = m_pDevice->CreateFXEffectFromFile("../../Win32TestBed/Particle.fx");
	m_pEffectParticle->SetValidateTechnique();

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

			m_pParticleTexture = m_pDevice->CreateTexture2D(RenderAPI::RESUSAGE_Default, texformat, png.width, png.height, png.buffer, png.line_pitch, png.height);
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

			m_pBoxTexture = m_pDevice->CreateTexture2D(RenderAPI::RESUSAGE_Default, texformat, bmp.width, bmp.height, nullptr, 0, 0);

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

void APITestBed::DrawBox(RenderAPI::TextureAddress address)
{
	RenderAPI::TextureSampler sampler;
	sampler.AddressU = sampler.AddressV = address;
	sampler.Filter = RenderAPI::FILTER_MinL_MagL_MipL;

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
			m_pContext->SetVertexBuffers(&(m_boxVBInfos[0]), m_boxVBInfos.size());
			m_pContext->SetIndexBuffer(m_pBoxIndexBuffer, 0);
			const int faceCount = 6 * 2;
			m_pContext->SetTexture(0, m_pBoxTexture);
			m_pContext->SetTextureSampler(0, sampler);
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
			m_pEffectParticle->SetTexture("g_particleTexture", m_pParticleTexture);
			m_pContext->SetVertexBuffers(&(m_particleVBInfos[0]), m_particleVBInfos.size());
			m_pContext->SetIndexBuffer(m_pParticleIB, 0);
			m_pEffectParticle->CommitChange();
			UploadParticlesAndCommitDrawcalls();

			m_pEffectParticle->EndPass();
		}
		m_pEffectParticle->End();
	}
}

void APITestBed::UploadParticlesAndCommitDrawcalls()
{
	int filledCount = 0;
	int particleCount = m_particleInstance.GetParticleCount();
	while (filledCount < particleCount)
	{

		const float kParticleSize = 0.1f;
		const float range = 1.5f;
		ParticleVertexD* vertices = (ParticleVertexD*)m_pParticleVBD->DiscardLock();
		if (vertices != nullptr)
		{
			filledCount += m_particleInstance.FillVertexBuffer(vertices, filledCount);
			m_pParticleVBD->Unlock();
		}

		m_pContext->DrawIndexed(RenderAPI::PRIMITIVE_TriangleList, 0, 0, filledCount * 2);
	}

}
