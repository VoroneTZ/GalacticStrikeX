const float4 vecSkill1;
const float4 vecViewPort;
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

//float4 PShader(float2 tex: TEXCOORD0) : COLOR
//{
//   float2 vS = vecSkill1.xy * 0.001f * float2(1, vecViewPort.x / vecViewPort.y);
//	
//	return tex2D(smpScreen, tex - 6 * vS) * 0.02040 +
//          tex2D(smpScreen, tex - 5 * vS) * 0.04081 +
//          tex2D(smpScreen, tex - 4 * vS) * 0.06122 +
//          tex2D(smpScreen, tex - 3 * vS) * 0.08163 +
//          tex2D(smpScreen, tex - 2 * vS) * 0.10204 +
//          tex2D(smpScreen, tex -   vS  ) * 0.12244 +
//          tex2D(smpScreen,        tex  ) * 0.14285 +
//          tex2D(smpScreen, tex +   vS  ) * 0.12244 +
//          tex2D(smpScreen, tex + 2 * vS) * 0.10204 +
//          tex2D(smpScreen, tex + 3 * vS) * 0.06122 +
//          tex2D(smpScreen, tex + 4 * vS) * 0.06122 +
//          tex2D(smpScreen, tex + 5 * vS) * 0.04081 +
//          tex2D(smpScreen, tex + 6 * vS) * 0.02040;
//}

float4 PShader(float2 tex: TEXCOORD0) : COLOR
{
   float2 vS = vecSkill1.xy * 0.001f * float2(1, vecViewPort.x / vecViewPort.y);
	
	return tex2D(smpScreen, tex - 6 * vS) * 0.002216 +
          tex2D(smpScreen, tex - 5 * vS) * 0.008764 +
          tex2D(smpScreen, tex - 4 * vS) * 0.026995 +
          tex2D(smpScreen, tex - 3 * vS) * 0.064759 +
          tex2D(smpScreen, tex - 2 * vS) * 0.120985 +
          tex2D(smpScreen, tex -   vS  ) * 0.176033 +
          tex2D(smpScreen,        tex  ) * 0.199471 +
          tex2D(smpScreen, tex +   vS  ) * 0.176033 +
          tex2D(smpScreen, tex + 2 * vS) * 0.120985 +
          tex2D(smpScreen, tex + 3 * vS) * 0.064759 +
          tex2D(smpScreen, tex + 4 * vS) * 0.026995 +
          tex2D(smpScreen, tex + 5 * vS) * 0.008764 +
          tex2D(smpScreen, tex + 6 * vS) * 0.002216;
}

technique tech_00
{
	pass pass_00
	{
		VertexShader = null;
		PixelShader = compile ps_1_0 PShader();
	}
}
