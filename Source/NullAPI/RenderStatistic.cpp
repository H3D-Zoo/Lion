#include "RenderStatistic.h" 
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include <vector>


unsigned int GetTextureLength(unsigned int w, unsigned int h, RenderAPI::TextureFormat format, bool bGenMipmap)
{
	unsigned int surfaceSize = 0;
	switch (format)
	{
	case RenderAPI::TEX_DXT1:
		surfaceSize = ((w + 3) / 4)*((h + 3) / 4) * 8;
		break;
	case RenderAPI::TEX_DXT3:
	case RenderAPI::TEX_DXT5:
		surfaceSize = ((w + 3) / 4)*((h + 3) / 4) * 16;
		break;
	case RenderAPI::TEX_ARGB:
	case RenderAPI::TEX_XRGB:
	case RenderAPI::TEX_D24S8:
	case RenderAPI::TEX_D24X8:
	case RenderAPI::TEX_D32:
		surfaceSize = ((w * 32 + 31) / 32)*((h * 32 + 31) / 32) * 4;
		break;

	case RenderAPI::TEX_R32F:
		surfaceSize = w*h * 4;
		break;
	case RenderAPI::TEX_D16:
		surfaceSize = w*h * 2;
		break;
	case RenderAPI::TEX_RG32F:
		surfaceSize = w*h * 4 * 2;
		break;
	case RenderAPI::TEX_ARGB32F:
		surfaceSize = w*h * 4 * 4;
		break;
	default:
		break;
	}
	if (bGenMipmap)
		surfaceSize = surfaceSize * 4 / 3;
	return surfaceSize;
}

void RenderStatistic::OnDrawcall(RenderAPI::Primitive primitive, unsigned int count)
{
	if (primitive == RenderAPI::PRIMITIVE_TriangleList)
	{
		m_data.NumFrameTriangles += count;
		m_data.NumFrameVertices += count * 3;
	}
	else if (primitive == RenderAPI::PRIMITIVE_TriangleFan)
	{
		m_data.NumFrameTriangles += count;
		m_data.NumFrameVertices += 1 + (count - 1) * 3;
	}
	else if (primitive == RenderAPI::PRIMITIVE_TriangleStrip)
	{
		m_data.NumFrameTriangles += count;
		m_data.NumFrameVertices += 3 + (count - 1);
	}
}

RenderStatistic::RenderStatistic()
	: m_clearStamp(1)
{
	Reset();
}

void RenderStatistic::Reset()
{
	memset(&m_data, 0, sizeof(m_data));
	++m_clearStamp;
}

void RenderStatistic::OnDraw(RenderAPI::Primitive primitive, unsigned int count)
{
	OnDrawcall(primitive, count);
	++m_data.NumDraw;
}

void RenderStatistic::OnDrawUp(RenderAPI::Primitive primitive, unsigned int count)
{
	OnDrawcall(primitive, count);
	++m_data.NumDrawUp;
}

void RenderStatistic::OnDrawIndexed(RenderAPI::Primitive primitive, unsigned int count)
{
	OnDrawcall(primitive, count);
	++m_data.NumDrawIndexed;
}

void RenderStatistic::OnDrawIndexedUp(RenderAPI::Primitive primitive, unsigned int count)
{
	OnDrawcall(primitive, count);
	++m_data.NumDrawIndexedUp;
}

void RenderStatistic::OnSetSourceStream(VertexBuffer* pVertexBuffer)
{
	if (pVertexBuffer->IsDynamic())
	{
		++m_data.NumSetDynamicStreamSource;
		m_data.ByteFrameDynamicVertexBuffer += pVertexBuffer->GetLength();
	}
	else
	{
		++m_data.NumSetStaticStreamSource;
		m_data.ByteFrameStaticVertexBuffer += pVertexBuffer->GetLength();
	}
}

void RenderStatistic::OnSetIndexBuffer(IndexBuffer * pIndexBuffer)
{
	if (pIndexBuffer->IsDynamic())
	{
		++m_data.NumSetDynamicIndices;
		m_data.ByteFrameDynamicIndexBuffer += pIndexBuffer->GetLength();
	}
	else
	{
		++m_data.NumSetStaticIndices;
		m_data.ByteFrameStaticIndexBuffer += pIndexBuffer->GetLength();
	}
}

void RenderStatistic::OnSetVertexDeclaration()
{
	++m_data.NumSetVertexDeclaration;
}

void RenderStatistic::OnSetTexture(RenderAPI::Texture* pTexture)
{
	++m_data.NumSetTexture;
	if (pTexture)
	{

		if (pTexture->IsCubemap())
		{
			unsigned int& ClearStamp = ((TextureCube*)pTexture)->ClearStamp;
			if(ClearStamp != m_clearStamp)
			{
				ClearStamp = m_clearStamp;
				++m_data.NumFrameTextures;
				OnSetTextureOnlySize(pTexture); 
			}
		}
		else
		{
			unsigned int& ClearStamp = ((Texture2D*)pTexture)->ClearStamp;
			if (ClearStamp != m_clearStamp)
			{
				ClearStamp = m_clearStamp;
				++m_data.NumFrameTextures;
				OnSetTextureOnlySize(pTexture);
			}
		}
	}
}

void RenderStatistic::OnSetTextureOnlySize(RenderAPI::Texture * pTexture)
{
	if (pTexture)
	{
		m_data.ByteFrameTexture += pTexture->GetLength();
	}
}

void RenderStatistic::OnSetVertexShader()
{
	++m_data.NumSetVertexShader;
}

void RenderStatistic::OnSetPixelShader()
{
	++m_data.NumSetPixelShader;
}

void RenderStatistic::OnSetRenderState()
{
	++m_data.NumSetRenderState;
}

void RenderStatistic::OnSetSamplerState()
{
	++m_data.NumSetSamplerState;
}

void RenderStatistic::OnSetSetTextureStageState()
{
	++m_data.NumSetTextureStageState;
}

void RenderStatistic::OnSetTransForm()
{
	++m_data.NumSetTransForm;
}

void RenderStatistic::OnBeginFx()
{
	++m_data.NumBeginFX;
}

void RenderStatistic::OnBeginPass()
{
	++m_data.NumBeginPass;
}

void RenderStatistic::OnEffectSetScalar()
{
	++m_data.NumEffectSetScalar;
}

void RenderStatistic::OnEffectSetValue(unsigned int sizeInByte)
{
	if (sizeInByte > 4)
	{
		++m_data.NumEffectSetVector;
	}
	else
	{
		OnEffectSetScalar();
	}
}

void RenderStatistic::OnEffectSetTexture()
{
	++m_data.NumEffectSetTexture;
}

void RenderStatistic::OnEffectSetMatrix(unsigned int count)
{
	m_data.NumEffectSetMatrix += count;
}