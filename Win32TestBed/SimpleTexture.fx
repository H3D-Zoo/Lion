float4x4 g_matWorld;
float4x4 g_matProj;
float2 g_texelOffset;
float2 g_textureSize;

texture2D g_texture;
sampler2D g_sampler = sampler_state
{
	Texture = <g_texture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = None;
};

struct VSInput
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct V2P
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
};

V2P VSSimpleTexture(VSInput INP)
{
	V2P OUT = (V2P)0;
	OUT.position = INP.position;
	OUT.texcoord = INP.texcoord;
	return OUT;
}

float4 PSSimpleTexture(V2P INP) : COLOR0
{
	return tex2D(g_sampler, INP.texcoord);
}

technique SimpleTexture
{
	pass
	{
		AlphaBlendEnable = FALSE;
		ZWriteEnable = FALSE;
		ZEnable = TRUE;

		StencilEnable = true;
		StencilMask = 0xFFFFFFFF;
		StencilMask = 0xFFFFFFFF;
		StencilRef = 1;
		StencilFunc = Equal;
		StencilPass = Keep;

		VertexShader = compile vs_1_0 VSSimpleTexture();
		PixelShader = compile ps_1_0 PSSimpleTexture();
	}
}

struct V2PStencil
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float2 screenpos : TEXCOORD1;
};

V2PStencil VSSimpleTextureStencil(VSInput INP)
{
	V2PStencil OUT = (V2PStencil)0;
	OUT.position = INP.position;
	OUT.texcoord = INP.texcoord;
	OUT.screenpos = (INP.position.xy - 0.25)  / 0.75 + g_texelOffset;
	return OUT;
}

float4 PSSimpleTextureStencil(V2PStencil INP) : COLOR0
{
	float2 screenPixelPos = INP.screenpos  / g_texelOffset;
	float2 intScreenPos = fmod(screenPixelPos, 2.0);
	intScreenPos = step(0.9, intScreenPos);
	float a = step(1.9, (intScreenPos.x + intScreenPos.y));
	return float4(INP.texcoord, 0, a);
}

technique SimpleTextureStencil
{
	pass
	{
		AlphaBlendEnable = FALSE;
		ZWriteEnable = FALSE;
		ZEnable = TRUE;

		StencilEnable = true;
		StencilRef = 0;
		StencilMask = 0xFFFFFFFF;
		StencilWriteMask = 0xFFFFFFFF;
		StencilFunc = Equal;
		StencilPass = Incr;

		VertexShader = compile vs_1_0 VSSimpleTextureStencil();
		PixelShader = compile ps_2_0 PSSimpleTextureStencil();
	}
}
