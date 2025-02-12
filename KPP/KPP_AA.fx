#define VeryHighQuality

float4 vecViewPort;
texture mtlSkin1;
float4 vecSkill1;

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
	tex *= vecSkill1.xy;
	
	float2 SmpOffsetFac = (vecViewPort.xy / (vecSkill1.z * 100));
	
	#ifdef VeryHighQuality
		return (tex2D(smpScreen, tex + float2(-vecViewPort.z * 1.5f, -vecViewPort.w * 1.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 0.5f, -vecViewPort.w * 1.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 0.5f, -vecViewPort.w * 1.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 1.5f, -vecViewPort.w * 1.5f) * SmpOffsetFac) +
		
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 1.5f, -vecViewPort.w * 0.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 0.5f, -vecViewPort.w * 0.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 0.5f, -vecViewPort.w * 0.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 1.5f, -vecViewPort.w * 0.5f) * SmpOffsetFac) +
		
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 1.5f,  vecViewPort.w * 0.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 0.5f,  vecViewPort.w * 0.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 0.5f,  vecViewPort.w * 0.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 1.5f,  vecViewPort.w * 0.5f) * SmpOffsetFac) +
		
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 1.5f,  vecViewPort.w * 1.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2(-vecViewPort.z * 0.5f,  vecViewPort.w * 1.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 0.5f,  vecViewPort.w * 1.5f) * SmpOffsetFac) +
		tex2D(smpScreen, tex + float2( vecViewPort.z * 1.5f,  vecViewPort.w * 1.5f) * SmpOffsetFac)) * 0.0625f;
	#else
		return tex2D(smpScreen, tex);
	#endif
}

technique tech_00
{
	pass pass_00
	{
		VertexShader = null;
		PixelShader = compile ps_3_0 PShader();
	}
}
