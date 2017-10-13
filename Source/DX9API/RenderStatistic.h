#pragma once
#include "../../RenderAPI/RenderAPI.h"
 
class VertexBuffer;
class IndexBuffer;

unsigned int GetTextureLength(unsigned int w, unsigned int h, RenderAPI::TextureFormat format,bool bGenMipmap);

class RenderStatistic
{
	unsigned int m_clearStamp;

	RenderAPI::RenderStatisticsData m_data;

	void OnDrawcall(RenderAPI::Primitive primitive, unsigned int count);
public:
	RenderStatistic();

	inline const RenderAPI::RenderStatisticsData& GetData() const { return m_data; }

	void Reset();

	void OnDraw(RenderAPI::Primitive primitive, unsigned int count);

	void OnDrawUp(RenderAPI::Primitive primitive, unsigned int count);

	void OnDrawIndexed(RenderAPI::Primitive primitive, unsigned int count);

	void OnDrawIndexedUp(RenderAPI::Primitive primitive, unsigned int count);

	void OnSetSourceStream(VertexBuffer* pVertexBuffer);

	void OnSetIndexBuffer(IndexBuffer* pIndexBuffer);

	void OnSetVertexDeclaration();

	void OnSetTexture(RenderAPI::Texture* pTexture);

	void OnSetTextureOnlySize(RenderAPI::Texture* pTexture);

	void OnSetCustomFVF();

	void OnSetVertexShader();

	void OnSetPixelShader();

	void OnSetRenderState();

	void OnSetSamplerState();

	void OnSetSetTextureStageState();

	void OnSetTransForm();

	void OnBeginFx();

	void OnBeginPass();

	void OnEffectSetScalar();

	void OnEffectSetValue(unsigned int sizeInByte);

	void OnEffectSetTexture();

	void OnEffectSetMatrix(unsigned int count);

	inline unsigned int GetCurrentStamp() const { return m_clearStamp; }
};
