float4x4 g_matWorld;
float4x4 g_matView;
float4x4 g_matProj;
struct VSInput
{
	float4 position : POSITION;
	float4 color : COLOR0;
};

struct V2P
{
	float4 position : POSITION;
	float4 color : TEXCOORD0;
};

V2P VSTintColor(VSInput INP)
{
	V2P OUT = (V2P)0;

	float4 outPos = mul(INP.position, g_matWorld);
	outPos = mul(outPos, g_matView);
	outPos = mul(outPos, g_matProj);

	OUT.position = outPos;
	OUT.color = INP.color;
	return OUT;
}

float4 PSTintColor(V2P INP) : COLOR0
{
	return float4(INP.color.rgb, 1.0f);
}

technique TintColor
{
	pass
	{
		VertexShader = compile vs_1_0 VSTintColor();
		PixelShader = compile ps_1_0 PSTintColor();
	}
}
