float4x4 gWorld;
float4x4 gLightViewProj;
float4x4 gBones[70];
 
DepthStencilState depthStencilState
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState rasterizerState
{
	FillMode = SOLID;
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader [STATIC]
//--------------------------------------------------------------------------------------
float4 ShadowMapVS(float3 position:POSITION) :SV_POSITION
{
	//TODO: return the position of the vertex in correct space (hint: seen from the view of the light)
	
	//STEP 1 - transform the input position into homogeneous coordinates
	float4 inputPos = float4(position, 1.0f);

	//STEP 2 - multiply the two matrices (World Matrix and Light View Projection) to obtain the light space matrix  // Light View Projection Matrix
	float4x4 lightSpaceMatrix = mul(gWorld, gLightViewProj);

	//SETP 3 - transform the position of the input postions, now in homogeneous coordinates, with the light space matrix
	float4 outputPos = mul(inputPos, lightSpaceMatrix);

	//return this outputPos
	return outputPos;
}

//--------------------------------------------------------------------------------------
// Vertex Shader [SKINNED]
//--------------------------------------------------------------------------------------
float4 ShadowMapVS_Skinned(float3 position:POSITION, float4 BoneIndices : BLENDINDICES, float4 BoneWeights : BLENDWEIGHTS) : SV_POSITION
{
	//TODO: return the position of the ANIMATED vertex in correct space (hint: seen from the view of the light)
	
	//STEP 1 - transform the input position into homogeneous coordinates
	float4 inputPos = float4(position, 1.0f);

	//STEP 2 - multiply the two matrices (World Matrix and Light View Projection) to obtain the light space matrix  // Light View Projection Matrix
	float4x4 lightSpaceMatrix = mul(gWorld, gLightViewProj);

	
	//STEP 3
	float4 transformedPosiiton;
	//one vertex can be attachted/skinned to up to 4 bones (we can add 4 indices and weights per vertex)
	//so, we goot to loop all possivel bones
	for (int i = 0; i < 4; i++)
	{
		//get the position of this vertex gBones matrix
		float index = BoneIndices[i];

		if (index >= 0)
		{
			//first we transform the homogenous inputPos by the gBone matrix
			float4 bonePos = mul(inputPos, gBones[index]);
			//second we transform that result 3d coordinate by the BoneWeight
			transformedPosiiton.xyz += mul(bonePos, BoneWeights[i]);
		}
	}
	transformedPosiiton.w = 1;

	//bonePos.w = 1;

	//SETP 4 - transform the position of the input postions, now in homogeneous coordinates, with the light space matrix
	float4 outputPos = mul(transformedPosiiton, lightSpaceMatrix);
	return outputPos;
}
 
//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------
void ShadowMapPS_VOID(float4 position:SV_POSITION){}

technique11 GenerateShadows
{
	pass P0
	{
		SetRasterizerState(rasterizerState);
	    SetDepthStencilState(depthStencilState, 0);
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}

technique11 GenerateShadows_Skinned
{
	pass P0
	{
		SetRasterizerState(rasterizerState);
		SetDepthStencilState(depthStencilState, 0);
		SetVertexShader(CompileShader(vs_4_0, ShadowMapVS_Skinned()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, ShadowMapPS_VOID()));
	}
}