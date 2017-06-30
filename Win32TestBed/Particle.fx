float4x4 g_matWorld;
float4x4 g_matView;
float4x4 g_matProj;
float4 g_cameraX;
float4 g_cameraY;

texture g_particleTexture;
sampler g_particleSampler = sampler_state
{
	Texture = <g_particleTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = None;
};

struct VSInput
{
	float4 position : POSITION;
	float4 color : COLOR0;
	float3 offset : TEXCOORD0;
	float2 texcoord : TEXCOORD1;
};

struct V2P
{
	float4 position : POSITION;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

V2P VSParticle(VSInput INP)
{
	V2P OUT = (V2P)0;
	float3 offsetVec = INP.offset.x * g_cameraX.xyz + INP.offset.y * g_cameraY.xyz;
	float4 outPos = mul(INP.position, g_matWorld) + float4(offsetVec.xyz, 0);
	outPos = mul(outPos, g_matView);
	outPos = mul(outPos, g_matProj);

	OUT.position = outPos;
	OUT.texcoord = INP.texcoord;
	OUT.color = float4(INP.color.rgb, 1);
	return OUT;
}

float4 PSParticle(V2P INP) : COLOR0
{
	return INP.color * tex2D(g_particleSampler, INP.texcoord);
}

technique Particle
{
	pass
	{
		AlphaBlendEnable = TRUE;
		SrcBlend = ONE;
		DestBlend = ONE;
		ZEnable = False;
		VertexShader = compile vs_1_0 VSParticle();
		PixelShader = compile ps_1_0 PSParticle();
	}
}
