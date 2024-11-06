float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

SamplerState samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;// or Mirror or Clamp or Border
	AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

SamplerComparisonState samCompairLinear
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerComparisonState samCompairPoint
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_POINT;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	//TODO: complete Vertex Shader
	//STEP 1 - transform the input position to homogenous coordinates
	float4 inputPos = float4(input.pos, 1.0f);
	//STEP 2 - apply the world view projection matrix on the inputPos, now in the correct coordinate system
	output.pos = mul(inputPos, gWorldViewProj);
	//STEP 3 - apply the world view projection matrix onto the input.normal, but, only with the float3x3 to avoid scalling
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	//STEP 4 - SAVE THE TextCoor
	output.texCoord = input.texCoord;
	//STEP 5
	// multiply the two matrices (World Matrix and Light View Projection) to obtain the light space matrix  // Light View Projection Matrix
	//float4x4 lightSpaceMatrix = mul(gWorld, gWorldViewProj_Light);
	//Hint: Don't forget to project our position to light clip space and store it in lPos
	output.lPos = mul(inputPos, gWorldViewProj_Light);


	//output.pos = mul(float4(input.pos, 1.0f), gWorldViewProj);
	//output.lPos = mul(float4(input.pos, 1.0f), gWorldViewProj_Light);
	//output.normal = normalize(mul(input.normal, (float3x3) gWorld));
	//output.texCoord = input.texCoord;

	return output;

}

float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)


	//The formula works by first adding 0.5 to both u and v to get the center of the texel in 
	//question. The + 0.5 term is added to ensure that the texture sampling occurs at the
	//center of the texel rather than at the texel corner.
	//Next, we divide both u and v by the corresponding dimension of the 
	//shadow map to get the texture coordinate in the range [0, 1]
	float2 textOffset = float2((u /*+ 0.5*/) / 4096, (v /*+ 0.5*/) / 2160);

	return textOffset;
}

float EvaluateShadowMap(float4 lpos)
{
	//TODO: complete
	//re-homogenize position after interpolation
	lpos.xyz /= lpos.w;

	//if position is not visible to the light - dont illuminate it
	//results in hard light frustum
    if (lpos.x < -1.0f || lpos.x > 1.0f ||
        lpos.y < -1.0f || lpos.y > 1.0f ||
        lpos.z < 0.0f || lpos.z > 1.0f)
        return 1.0f;

	////transform clip space coords to texture space coords (-1:1 to 0:1)
	lpos.x = lpos.x / 2 + 0.5;
	lpos.y = lpos.y / -2 + 0.5;
	lpos.z -= gShadowMapBias;


	float shadowFactor = 0;

	//Average out
	for (float y = -1.5f; y <= 1.5f; y += 1.0)
	{
		for (float x = -1.5f; x <= 1.5f; x += 1.0)
		{
			shadowFactor += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
			if (shadowFactor < lpos.z)
			{
				return 0.5f;
			}

		}
	}

	shadowFactor /= 16.0;

	return shadowFactor;

	////sample shadow map - point sampler
	//float shadowMapDepth = shadowMap.Sample(pointSampler, input.lpos.xy).r;

	////if clip space z value greater than shadow map value then pixel is in shadow
	//if (shadowMapDepth < input.lpos.z) 
	//	return ambient;
	//return 1.0f;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb * shadowValue , color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

