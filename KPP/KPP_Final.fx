// Some Settings:

#define N 0.275f
#define V 0.65f

//

const float4 vecSkill1;
const float4 vecSkill5;
const float4 vecSkill9;
const float4 vecViewPort;

const texture mtlSkin1;
const texture mtlSkin2;
const texture mtlSkin3;
const texture tBlurL_bmap;
const texture tNoise_bmap;

sampler2D smpA = sampler_state
{
	Texture = <mtlSkin1>;

	MinFilter	= linear;
	MagFilter	= linear;
	MipFilter	= linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler2D smpB = sampler_state
{
	Texture = <mtlSkin2>;

	MinFilter	= linear;
	MagFilter	= linear;
	MipFilter	= linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler2D smpC = sampler_state
{
	Texture = <mtlSkin3>;

	MinFilter	= linear;
	MagFilter	= linear;
	MipFilter	= linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler2D smpD = sampler_state
{
	Texture = <tBlurL_bmap>;

	MinFilter	= linear;
	MagFilter	= linear;
	MipFilter	= linear;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler2D smpE = sampler_state
{
	Texture = <tNoise_bmap>;

	MinFilter	= linear;
	MagFilter	= linear;
	MipFilter	= linear;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 RGB_to_RGBA(float3 RGB_In)
{
	float alpha = saturate(max(max(RGB_In.r, RGB_In.g), RGB_In.b));
	return float4(RGB_In / alpha, alpha);
}

float4 AlphaBlend(float4 Bottom, float4 Top)
{
	return float4(Bottom.rgb * (1.f - Top.a) + Top.rgb * Top.a, Top.a + (1.f - Top.a) * Bottom.a);
}

float4 tex2D_blur(sampler2D smp, float2 tex, float dist) // simple linear 4 sample blur
{
	return (	tex2D(smp, tex + float2(-vecViewPort.z, -vecViewPort.w) * dist) + 
				tex2D(smp, tex + float2( vecViewPort.z, -vecViewPort.w) * dist) + 
				tex2D(smp, tex + float2(-vecViewPort.z,  vecViewPort.w) * dist) + 
				tex2D(smp, tex + float2( vecViewPort.z,  vecViewPort.w) * dist) ) * 0.25;
}

#define TMR(x,y) (x>y?y+((2-1/((x-y)/((1-y)*2)+y))*(0.5*(1-y))):x)

float4 PShader(float2 in_tex: TEXCOORD0) : COLOR
{
	float2 tex = in_tex / vecSkill1.zw;
	
	float3 x = tex2D(smpA, tex).rgb;
	
	x = lerp((x.r+x.g+x.b)*0.3333,x,vecSkill9.x);
	
	x = lerp(x,TMR(x, 0.5),0.6);
	
	float4 A = 0;
	A = tex2D_blur(smpC, tex, 4);
	A.rgb += (tex2D_blur(smpB, tex, 2).rgb * 0.65);
	
	A.rgb = lerp(A.rgb, pow(TMR(A.rgb * 0.75, 0.5), 0.8) * 1.125, 1);
	
	x = saturate(x + A.rgb * saturate(1 - saturate(x)));
	
	A = tex2D(smpD, tex);
	x = saturate(x + A.rgb * saturate(1 - saturate(x)));
	
	float v = 1 - distance(float2(0.5f, 0.5f), tex) * vecSkill5.w;
	v = saturate(v * (2.0f - v));
	float n = (1 - pow((x.r + x.g + x.b) * 0.3333f, 0.5f)) * vecSkill5.z;
	x = saturate(x * (v + (tex2D(smpE, tex.xy * vecViewPort.xy / 512.0f + vecSkill5.xy) - 0.5) * n));
	x = lerp(x, (saturate(pow(x, 2)) * (1.0f - x) + saturate(pow(x, 0.1f) - 0.2f) * x * 1.2f) * saturate(1.0f - x * 1.2f) + x * saturate(x * 1.2f), 2.25);
	
	float4 Color = float4(pow(x, vecSkill9.y<0?(-1/vecSkill9.y):(vecSkill9.y+1)), 1);
	
	return Color;
}

technique tech_00
{
	pass pass_00
	{
		VertexShader = Null;
		PixelShader = compile ps_3_0 PShader();
	}
}
