
#define Use_Custom_Scale

//==========

#ifdef Use_Custom_Scale
	const float4 vecSkill1;
#endif

const float4 vecViewPort;

static const float2 pxOffset[8] = 
{
	{ 1, 1 },
	{ 3, 1 },
	{ 1, 3 },
	{ 3, 3 },
	{ 2,-1 },
	{-1, 2 },
	{ 5, 2 },
	{ 2, 5 }
};

//==========

const texture mtlSkin1;
sampler2D smpTex = sampler_state
{
	Texture = <mtlSkin1>;
	
	MipFilter = Linear;
	MagFilter = Linear;
	MinFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

//==========

float4 PShader(in float2 tex : TEXCOORD0) : COLOR0
{
	#ifdef Use_Custom_Scale
		tex = (tex - 0.5 * vecViewPort.zw) * vecSkill1.xy;
	#else
		tex = (tex - 0.5 * vecViewPort.zw) * 4;
	#endif
	
	float4 color = 0;
	color.a = 1;
	
	for(int i = 0; i < 8; i++)
	{
		color.rgb += tex2D(smpTex, tex + pxOffset[i] * vecViewPort.zw).rgb;
	}
	
	color.rgb /= 8;
	
	return float4(color.rgb, 1);
}

//==========

technique Tech_PP
{
	pass Pass_1
	{
		PixelShader = compile ps_3_0 PShader();
	}
}