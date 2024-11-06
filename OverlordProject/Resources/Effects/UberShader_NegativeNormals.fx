/*
******************
* DAE Ubershader *
******************

**This Shader Contains:

- Diffuse (Texture & Color)
	- Regular Diffuse
- Specular
	- Specular Level (Texture & Value)
	- Shininess (Value)
	- Models
		- Blinn
		- Phong
- Ambient (Color)
- EnvironmentMapping (CubeMap)
	- Reflection + Fresnel Falloff
	- Refraction
- Normal (Texture)
- Opacity (Texture & Value)

-Techniques
	- WithAlphaBlending
	- WithoutAlphaBlending
*/

//GLOBAL MATRICES
//***************
// The World View Projection Matrix
float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
// The ViewInverse Matrix - the third row contains the camera position!
float4x4 gMatrixViewInverse : VIEWINVERSE;
// The World Matrix
float4x4 gMatrixWorld : WORLD;

//STATES
//******
RasterizerState gRS_FrontCulling 
{ 
	CullMode = NONE; 
};

BlendState gBS_EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

//SAMPLER STATES
//**************
SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//LIGHT
//*****
float3 gLightDirection :DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(0.551f, -0.534f, 0.641f);

//DIFFUSE
//*******
bool gUseTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Bool";
> = false;

float4 gColorDiffuse
<
	string UIName = "Diffuse Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

Texture2D gTextureDiffuse
<
	string UIName = "Diffuse Texture";
	//string ResourceName =  "CobbleStone_NormalMap.dds";
	string UIWidget = "Texture";
>;

//SPECULAR
//********
float4 gColorSpecular
<
	string UIName = "Specular Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

Texture2D gTextureSpecularIntensity
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Texture";
>;

bool gUseTextureSpecularIntensity
<
	string UIName = "Specular Level Texture";
	string UIWidget = "Bool";
> = false;

int gShininess
<
	string UIName = "Shininess";
	string UIWidget = "Slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1f;
> = 15;

//AMBIENT
//*******
float4 gColorAmbient
<
	string UIName = "Ambient Color";
	string UIWidget = "Color";
> = float4(0,0,0,1);

float gAmbientIntensity
<
	string UIName = "Ambient Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
>  = 0.0f;

//NORMAL MAPPING
//**************
bool gFlipGreenChannel
<
	string UIName = "Flip Green Channel";
	string UIWidget = "Bool";
> = false;

bool gUseTextureNormal
<
	string UIName = "Normal Mapping";
	string UIWidget = "Bool";
> = false;

Texture2D gTextureNormal
<
	string UIName = "Normal Texture";
	string UIWidget = "Texture";
>;

//ENVIRONMENT MAPPING
//*******************
TextureCube gCubeEnvironment
<
	string UIName = "Environment Cube";
	string ResourceType = "Cube";
>;

bool gUseEnvironmentMapping
<
	string UIName = "Environment Mapping";
	string UIWidget = "Bool";
> = false;

float gReflectionStrength
<
	string UIName = "Reflection Strength";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.1;
>  = 0.0f;

float gRefractionStrength
<
	string UIName = "Refraction Strength";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.1;
>  = 0.0f;

float gRefractionIndex
<
	string UIName = "Refraction Index";
>  = 0.3f;

//FRESNEL FALLOFF
//***************
bool gUseFresnelFalloff
<
	string UIName = "Fresnel FallOff";
	string UIWidget = "Bool";
> = false;


float4 gColorFresnel
<
	string UIName = "Fresnel Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

float gFresnelPower
<
	string UIName = "Fresnel Power";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 1.0f;

float gFresnelMultiplier
<
	string UIName = "Fresnel Multiplier";
	string UIWidget = "slider";
	float UIMin = 1;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 1.0;

float gFresnelHardness
<
	string UIName = "Fresnel Hardness";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 100;
	float UIStep = 0.1;
>  = 0;

//OPACITY
//***************
float gOpacityIntensity
<
	string UIName = "Opacity Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
>  = 1.0f;

bool gTextureOpacityIntensity
<
	string UIName = "Opacity Map";
	string UIWidget = "Bool";
> = false;

Texture2D gTextureOpacity
<
	string UIName = "Opacity Map";
	string UIWidget = "Texture";
>;


//SPECULAR MODELS
//***************
bool gUseSpecularBlinn
<
	string UIName = "Specular Blinn";
	string UIWidget = "Bool";
> = false;

bool gUseSpecularPhong
<
	string UIName = "Specular Phong";
	string UIWidget = "Bool";
> = false;

//VS IN & OUT
//***********
struct VS_Input
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition: COLOR0;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

float3 CalculateSpecularBlinn(float3 viewDirection, float3 normal, float2 texCoord)
{
    //Specular
    float3 halfVector = -normalize(viewDirection + gLightDirection);
    float specularStrength = dot(halfVector, normal);
    specularStrength = saturate(specularStrength);
    specularStrength = pow(specularStrength, gShininess);
        
    if(gUseTextureSpecularIntensity)
    {
        float specularIntensity = gTextureSpecularIntensity.Sample(gTextureSampler,texCoord).r;
        float specularBlinn = specularStrength * specularIntensity;
        float3 specColor = float3(specularBlinn, specularBlinn, specularBlinn);
        return (specColor * gColorSpecular);
    }
        
    float3 specColor = specularStrength * gColorSpecular;
    return specColor;  
}

float3 CalculateSpecularPhong(float3 viewDirection, float3 normal, float2 texCoord)
{	
	float3 specColor = (0, 0, 0);	
	
	float3 reflectedLight = reflect(-gLightDirection, normal);
	float specularStrength = max(0, dot(reflectedLight, viewDirection));
	specularStrength = saturate(specularStrength);
	specularStrength = pow(specularStrength, gShininess);
		
	if(gUseTextureSpecularIntensity)
	{
		float specularIntensity = gTextureSpecularIntensity.Sample(gTextureSampler,texCoord).r;
		float specularPhong = specularStrength * specularIntensity;
		float3 specColor = float3(specularPhong, specularPhong, specularPhong);
		return (specColor * gColorSpecular);
	}	
	specColor = specularStrength * gColorSpecular;
	return specColor;
}

float3 CalculateSpecular(float3 viewDirection, float3 normal, float2 texCoord)
{
	if(gUseSpecularBlinn)
	{
		float3 specularBlinn = CalculateSpecularBlinn(viewDirection, normal, texCoord);
		return specularBlinn;
	}
	if(gUseSpecularPhong)
	{
		float3 specularPhong = CalculateSpecularPhong(viewDirection, normal, texCoord);
		return specularPhong;
	}
	return float3(0, 0, 0);
}

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	// Step 2
	float3 binormal = normalize(cross(normal, tangent));
	// Step 3
	float3x3 localAxis = float3x3(tangent, binormal, normal);
	// Step 4
	float3 sampledNormal = gTextureNormal.Sample(gTextureSampler, texCoord);
	float x = lerp(-1, 1, sampledNormal.x);
	float y = lerp(-1, 1, sampledNormal.y);
	float z = lerp(-1, 1, sampledNormal.z);
	sampledNormal = float3(x, y, z);
	
	float3 newNormal = mul(sampledNormal, localAxis);
	
	return newNormal;
}

float3 DiffuseDio(float3 normal, float2 texCoord)
{
	float3 diffColor = gColorDiffuse;

	if(gUseTextureDiffuse)
	{
		float diffuseLightValue = max(dot(-normal,gLightDirection), 0);
		diffColor = gTextureDiffuse.Sample( gTextureSampler, texCoord) * diffuseLightValue;
		return diffColor;
	}
	float diffuseLightValue = max(dot(-normal,gLightDirection), 0);
	diffColor = gTextureDiffuse.Sample( gTextureSampler, texCoord) * diffuseLightValue;
	return diffColor * gColorDiffuse;
}

float3 DiffuseTan(float3 normal, float2 texCoord)
{
	float diffuseLightValue = max(0,dot(-normal, gLightDirection));

    float3 diffcolor = gColorDiffuse * diffuseLightValue;

    if (gUseTextureDiffuse)
        return gTextureDiffuse.Sample(gTextureSampler, texCoord) * diffcolor;

    return diffcolor;
}


float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	//return DiffuseDio(normal, texCoord);
	return DiffuseTan(normal, texCoord);
}

float3 CalculateFresnelFalloff(float3 normal, float3 viewDirection, float3 environmentColor)
{
	if(gUseFresnelFalloff == false)
		return environmentColor;

	if(gUseEnvironmentMapping == false)
		return gColorFresnel;
	
	
	float3 fresnel = ((1-saturate(abs(dot(normal, viewDirection)))));
	fresnel = pow(fresnel,gFresnelPower);
	fresnel = fresnel * gFresnelMultiplier;
	
	float3 fresnelMask = (1-saturate(dot(float3(0, -1, 0), normal)));
	fresnelMask = pow(fresnelMask,gFresnelHardness);

	return float3(fresnel * fresnelMask * environmentColor * gColorFresnel);
}

float3 Env1(float3 viewDirection, float3 normal)
{
	float3 envColor = float3(0, 0, 0);
	if(gUseEnvironmentMapping)
	{
		float3 reflectedVector = reflect(viewDirection, normal);
		float3 sampleColor = gCubeEnvironment.Sample( gTextureSampler, reflectedVector);
		return sampleColor * envColor;
	}
	return envColor;
}

float3 Env2(float3 viewDirection, float3 normal)
{
	float3 envColor = float3(0, 0, 0);
	
	if(gUseEnvironmentMapping)
	{
		float3 reflectedVector = reflect(viewDirection, normal);
		float3 refractedVector = refract(viewDirection, normal, gRefractionIndex);
		
		float3 sampleReflected = gCubeEnvironment.Sample( gTextureSampler, reflectedVector) * gReflectionStrength;
		float3 sampleRefracted = gCubeEnvironment.Sample( gTextureSampler, refractedVector) * gRefractionStrength;

		envColor = sampleReflected + sampleRefracted;
	}
		
		//float3 sampleColor = gCubeEnvironment.Sample( gTextureSampler, reflectedVector);
		//return sampleColor * envColor;
	return envColor;
}

float3 CalculateEnvironment(float3 viewDirection, float3 normal)
{	
	//return Env1(viewDirection, normal);
	return Env2(viewDirection, normal);
}

float CalculateOpacity(float2 texCoord)
{
	float opacity = gOpacityIntensity;
	if(gTextureOpacityIntensity)
	{
		opacity = gTextureOpacity.Sample( gTextureSampler, texCoord);
	}
	return opacity;
}

// The main vertex shader
VS_Output MainVS(VS_Input input) 
{
	VS_Output output = (VS_Output)0;
	// Step 1
	output.Position = mul(float4(input.Position,1), gMatrixWVP);
	// Step 2
	output.Normal = mul(normalize(-input.Normal), (float3x3)gMatrixWorld);
	// Step 3
	output.TexCoord = input.TexCoord;
	// Step 4
	output.Tangent = mul (normalize(input.Tangent), (float3x3)gMatrixWorld);
	// Step 5: Return output
	return output;
	
	
	return output;
}

// The main pixel shader
float4 MainPS(VS_Output input) : SV_TARGET {
	// NORMALIZE
	input.Normal = normalize(input.Normal);
	input.Tangent = normalize(input.Tangent);
	
	float3 viewDirection = normalize(input.WorldPosition.xyz - gMatrixViewInverse[3].xyz);
	
	//NORMAL
	float3 newNormal = CalculateNormal(input.Tangent, input.Normal, input.TexCoord);
		
	//SPECULAR
	float3 specColor = CalculateSpecular(viewDirection, newNormal, input.TexCoord);
		
	//DIFFUSE
	float3 diffColor = CalculateDiffuse(input.Normal, input.TexCoord);
		
	//AMBIENT
	float3 ambientColor = gColorAmbient * gAmbientIntensity;
		
	//ENVIRONMENT MAPPING
	float3 environmentColor = CalculateEnvironment(viewDirection, newNormal);
	
	//FRESNEL FALLOFF
	environmentColor = CalculateFresnelFalloff(newNormal, viewDirection, environmentColor);
		
	//FINAL COLOR CALCULATION
	float3 finalColor = diffColor + specColor + environmentColor + ambientColor;
	
	//OPACITY
	float opacity = CalculateOpacity(input.TexCoord);
	
	return float4(finalColor,opacity);
}

// Default Technique
technique10 WithAlphaBlending {
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetBlendState(gBS_EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

// Default Technique
technique10 WithoutAlphaBlending {
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}

