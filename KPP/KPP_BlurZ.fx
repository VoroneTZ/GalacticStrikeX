#define Samples 8

float4 vecSkill1;
float4 vecViewPort;
texture mtlSkin1;

sampler2D smpScreen = sampler_state
{
   Texture = <mtlSkin1>;

   MinFilter	= linear;
   MagFilter	= linear;
   MipFilter	= linear;
   
   AddressU = Clamp;
   AddressV = Clamp;
};

float4 PShader(float2 tex: TEXCOORD0) : COLOR
{
	if(vecSkill1.z < 0.1) return tex2D(smpScreen, tex);
	
   float2 vS = vecSkill1.z * 0.001f;
	
	float fade = saturate(vecSkill1.z * 0.1f);
	
	float4 Color = 0;
	
	for(int i = 0; i < Samples; i++)
	{
		Color += tex2D(smpScreen, ((tex - vecSkill1.xy) * (1 - vS * i / Samples)) + vecSkill1.xy);
	}
	
	Color /= Samples;
	
	return Color;
}

technique tech_00
{
	pass pass_00
	{
		VertexShader = null;
		PixelShader = compile ps_2_0 PShader();
	}
}
