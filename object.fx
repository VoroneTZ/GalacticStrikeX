/*

Skin1:
RGB = Color
A = Inverted Glowmap - only if luminance is enabled (0% alpha = full glow; 100% alpha = no glow)

Skin2:
RG = X- and Y-Normal in tangent space (just like every common normalmap)
BA = Not Used

*/

//////////////////////////////////////////////////////////////////////
// Material Settings:

#define EnableAlphaClip 0.0f // If this is enabled and the alpha of the pixel is lower than or equal this value, it will be transparent

//> Lighting
#define LightBrightnessFactor 2.0f // brightness-factor of all lights, needs to be set a bit higher
//


//> Specular Lighting
#define EnableSpecularLighting // Use specular lighting

#define SpecularIntensity 0.375f // Specular intensity, when using low "gloss"-values decrease this value, too (or it'll get very bright)
#define SpecularPower 10 // Specular "gloss"; affects intensity, too

/*
good values are:

Low gloss factor
SpecularIntensity: 	0.3f
SpecularPower: 		3

medium gloss factor
SpecularIntensity: 	0.375f
SpecularPower: 		10

high gloss factor
SpecularIntensity: 	0.5f
SpecularPower: 		20

very high gloss factor (good for wet surfaces)
SpecularIntensity: 	1.25f
SpecularPower: 		50

*/

//


//> Normal Mapping
#define EnableNormalmapping // Use normalmapping? (second skin)

#define NormalmappingStrength 0.5f // Strength of the normalmap
//


//> Luminance
#define EnableLuminance // Use the first skin's alpha as luminance map: 100% alpha = no glow; 0% alpha = full glow

#define LuminanceAlphaBlend // Use the glow as an "overlay" in stead of additive-blending it
//


// End of Material Settings
//////////////////////////////////////////////////////////////////////

#define CalcSpecular(normal,view,light,matPower) pow(saturate(dot(normal,normalize(light+view))),matPower*4) // reflection-model for specular reflections; this one is blinn-phong

#define SpotlightArc 8.0f // Spotlight-Cone-Arc,  ~(180 / Angle)^2   ||   4 = 90°;  16 = 45°
#define SpotlightArc2 7.0f // == SpotlightArc - 1

const float4x4 matWorldViewProj;
const float4x4 matWorld;
const float4 vecColor;
const float4 vecViewPos;
const float4 vecLightPos[8];
const float4 vecLightDir[8];
const float4 vecLightColor[8];
const float fAmbient;

texture entSkin1; // first skin of the entity, used as main color
sampler smpColor = sampler_state { Texture = <entSkin1>; MipFilter = Linear; MinFilter = Linear; MagFilter = Linear; AddressU = Wrap; AddressV = Wrap; };

#ifdef EnableNormalmapping
	float3x3 matTangent;
	texture entSkin2; // second skin of the entity, used as normalmap
	sampler smpNormal = sampler_state { Texture = <entSkin2>; MipFilter = Linear; MinFilter = Linear; MagFilter = Linear; AddressU = Wrap; AddressV = Wrap; };
#endif

//////////////////////////////////////////////////////////////////////
struct VS_OUT // the vertex-shader's output, gets passed to the pixel-shader
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
	float3 wPos : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	
	#ifdef EnableSpecularLighting
		float3 ViewDir : TEXCOORD3;
	#endif
	
	#ifdef EnableNormalmapping
		float3 Tangent : TEXCOORD4;
		float3 Binormal : TEXCOORD5;
	#endif
};

VS_OUT VShader(
	float4 inPos : POSITION,
	float3 inNormal : NORMAL,
	float2 inTex : TEXCOORD0
	#ifdef EnableNormalmapping
		,float3 inTangent : TEXCOORD2
	#endif
	)
{
	VS_OUT Out;
	
	// Transform the vertex from object space to clip space: 
	Out.Pos = mul(inPos, matWorldViewProj);
	Out.Tex = inTex;
	Out.wPos = mul(inPos, matWorld);
	
	#ifdef EnableNormalmapping
		Out.Tangent = mul(inTangent, matWorld);
		Out.Binormal = -mul(cross(inTangent, inNormal), matWorld);
	#endif
	
	Out.Normal = mul(inNormal, matWorld);
	
	#ifdef EnableSpecularLighting
		Out.ViewDir = vecViewPos - Out.wPos;
	#endif
	
	return Out;
}

float4 PShader(VS_OUT In): COLOR
{
	float4 Color = tex2D(smpColor, In.Tex);
	
	//
	
	#ifdef EnableAlphaClip
		if(Color.a <= EnableAlphaClip)
		{
			Color.a = -1;
			clip(Color.a);
			return float4(0, 0, 0, 0);
		}
	#endif
	
	//
	
	#ifdef EnableNormalmapping
//		float3 Normal = normalize(float3((tex2D(smpNormal, In.Tex).rg * 2 - 1), 1 / NormalmappingStrength));
		float3 Normal = tex2D(smpNormal, In.Tex).rgb * 2 - 1;
		Normal = normalize(In.Normal + (Normal.r * In.Tangent + Normal.g * In.Binormal));
	#else
		float3 Normal = In.Normal;
	#endif
	
	float3 Diffuse = fAmbient;
	
	#ifdef EnableSpecularLighting
		float4 Specular = 0.f;
		
		In.ViewDir = normalize(In.ViewDir);
	#endif
	
	float3 LightDir;
	float dott;
	
	float attenuation;
	
	float3 LightColor;
	
	for(int i = 0; i < 8; i++)
	{
		LightColor = vecLightColor[i].rgb * LightBrightnessFactor;
		
		LightDir = normalize(vecLightPos[i].xyz - In.wPos.xyz);
		
		if(vecLightDir[i].w > 0) // Spotlight?
			attenuation = saturate(saturate(dot(-normalize(vecLightDir[i].xyz), LightDir) * SpotlightArc - SpotlightArc2) - distance(vecLightPos[i].xyz, In.wPos.xyz) / vecLightPos[i].w);
		else
			attenuation = 1.f - saturate(distance(vecLightPos[i].xyz, In.wPos.xyz) / vecLightPos[i].w);
		
		if(attenuation > 0)
		{
			dott = dot(Normal, LightDir);
			
			if(dott > 0)
			{
				Diffuse += saturate(dott) * LightColor * attenuation;
				
				#ifdef EnableSpecularLighting
					Specular.rgb += CalcSpecular(Normal, In.ViewDir, LightDir, SpecularPower) * SpecularIntensity * LightColor * attenuation;
				#endif
			}
		}
	}
	
	//
	
	Color.rgb *= Diffuse;
	
	#ifdef EnableLuminance
		#ifdef LuminanceAlphaBlend
			Color.rgb = Color.rgb * Color.a + vecColor.rgb * (1.f - Color.a);
		#else
			Color.rgb += vecColor.rgb * (1.f - Color.a);
		#endif
	#endif
	
	#ifdef EnableSpecularLighting
		Color.rgb += Specular;
	#endif
	
	Color.a = 1;
	
	return Color;
}


technique tech
{
	pass one
	{
		ZWriteEnable = true;
		ZEnable = true;
		ZFunc  = Less;
		AlphaBlendEnable = true;
		FillMode = Solid;
		CullMode = CCW;
		
		VertexShader = compile vs_3_0 VShader();
		PixelShader = compile ps_3_0 PShader();
	}
}

technique fallback { pass one { } }