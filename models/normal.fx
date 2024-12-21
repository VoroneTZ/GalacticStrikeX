/***********************************************************************************************
/	Copyright 2006	by Taco Cohen. All rights reserved	
/  2007 Modified and adapted to A7 (jcl)						
/***********************************************************************************************/

/***********************************************************************************************
/	Global Variables:
/***********************************************************************************************/
// Tweakables:
static const float AmbientIntensity	= 1.0f;					// The intensity of the ambient light.
static const float DiffuseIntensity = 1.0f;					// The intensity of the diffuse light.
static const float SpecularIntensity = 1.0f;				// The intensity of the specular light.
static const float SpecularPower = 8.0f;					// The specular power. Used as 'glossyness' factor.
static const float4 SunColor = {0.9f, 0.9f, 0.5f, 1.0f};	// Color vector of the sunlight.

// Application fed data:
const float4x4 matWorldViewProj;	// World*view*projection matrix.
const float4x4 matWorld;			// World matrix.
const float4 vecAmbient;			// Ambient color.
const float4 vecSunDir;				// The sun direction vector.
const float4 vecViewPos;			// View position.

float3x3 matTangent;

texture entSkin1; 							// Color map.
sampler ColorMapSampler = sampler_state		// Color map sampler.
{
	Texture = <entSkin1>;
	MipFilter = LINEAR;
};

texture entSkin2; 							// Normal map.
sampler NormalMapSampler = sampler_state	// Normal map sampler.
{
	Texture = <entSkin2>;
	MipFilter = NONE;
};


/***********************************************************************************************
/	Vertex Shader:
/***********************************************************************************************/
void NormalMapVS(	in float4 InPos	: POSITION,
					in float3 InNormal	: NORMAL,
					in float2 InTex		: TEXCOORD0,
					in float4 InTangent	: TEXCOORD2,
					
					out float4 OutPos	: POSITION,
					out float2 OutTex	: TEXCOORD0,
					out float3 OutViewDir: TEXCOORD1,
					out float3 OutSunDir: TEXCOORD2)
{
	// Transform the vertex from object space to clip space:
	OutPos = mul(InPos, matWorldViewProj);
	
	// Pass the texture coordinate to the pixel shader:
	OutTex = InTex;
	
	// Compute 3x3 matrix to transform from world space to tangent space:
	matTangent[0] = mul(InTangent.xyz, matWorld);
	//matTangent[1] = mul(cross(InTangent.xyz,InNormal), matWorld);
	matTangent[1] = mul(cross(InTangent.xyz,InNormal)*InTangent.w, matWorld);
	matTangent[2] = mul(InNormal, matWorld);
	
	// Calculate the view direction vector in tangent space:
	OutViewDir = normalize(mul(matTangent, vecViewPos - mul(InPos, matWorld)));
	
	// Calculate the light direction vector in tangent space:
	OutSunDir = normalize(mul(matTangent, -vecSunDir));
}


/***********************************************************************************************
/	Pixel Shader:
/***********************************************************************************************/
float4 NormalMapPS(	in float2 InTex		: TEXCOORD0,
					in float3 InViewDir	: TEXCOORD1,
					in float3 InSunDir	: TEXCOORD2) : COLOR
{
	// Read the normal from the normal map and convert from [0..1] to [-1..1] range
	float3 BumpNormal = 2 * tex2D(NormalMapSampler, InTex) - 1;
	
	// Calculate the ambient term:
	float4 Ambient = AmbientIntensity * vecAmbient;
	
	// Calculate the diffuse term:
	float4 Diffuse = DiffuseIntensity * saturate(dot(InSunDir, BumpNormal));
	Diffuse *= SunColor;
	
	// Calculate the reflection vector:
	float3 R = normalize(2 * dot(BumpNormal, InSunDir) * BumpNormal - InSunDir);
	
	// Calculate the specular term:
	InViewDir = normalize(InViewDir);
	float Specular = pow(saturate(dot(R, InViewDir)), SpecularPower) * SpecularIntensity;
	
	// Fetch the pixel color from the color map:
	float4 Color = tex2D(ColorMapSampler, InTex);
	
	// Calculate final color:
	return (Ambient + Diffuse + Specular) * Color;
}

/***********************************************************************************************
/	Technique:
/***********************************************************************************************/
technique SpecularTechnique
{
	pass P0
	{
		VertexShader = compile vs_2_0 NormalMapVS();
		PixelShader  = compile ps_2_0 NormalMapPS();
	}
}