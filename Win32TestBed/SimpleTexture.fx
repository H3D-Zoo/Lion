float4x4 g_matWorld;
float4x4 g_matProj;
float2 g_texelOffset;

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

		VertexShader = compile vs_1_0 VSSimpleTexture();
		PixelShader = compile ps_1_0 PSSimpleTexture();
	}
}
