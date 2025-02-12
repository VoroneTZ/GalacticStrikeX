
texture mtlSkin1;
float4 vecSkill1;

sampler2D smpScreen = sampler_state
{
   Texture = <mtlSkin1>;

   MinFilter	= linear;
   MagFilter	= linear;
   MipFilter	= linear;
   
   AddressU = Border;
   AddressV = Border;
};

float3 do_cut(float3 col)
{
	return max(col.rgb - vecSkill1.x, 0) * vecSkill1.y;
}

float3 TonemapR(float3 Color, float curve_point)
{
	return Color > curve_point ? curve_point + ((2 - 1 / ((Color - curve_point) / ((1 - curve_point) * 2) + 0.5)) * (0.5 * (1 - curve_point))) : Color;
}

float4 PShader(float2 tex: TEXCOORD0) : COLOR
{
	float3 color = tex2D(smpScreen, tex).rgb;
	
	color = do_cut(color);
	
	color = lerp(color, TonemapR(color, 0.3), 0.5);
	
	color = lerp((color.r + color.g + color.b) * 0.3333, color, vecSkill1.z);
	
	return float4(color, 1);
}

technique tech_00
{
	pass pass_00
	{
		VertexShader = null;
		PixelShader = compile ps_3_0 PShader();
	}
}
