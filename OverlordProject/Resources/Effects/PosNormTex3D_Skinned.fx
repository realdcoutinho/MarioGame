float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70];


Texture2D gDiffuseMap;
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = FRONT;
};

struct VS_INPUT{ //extend input
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 blendWeights : BLENDWEIGHTS;
    float4 blendIndices : BLENDINDICES;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
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

BlendState NoBlending
{
	BlendEnable[0] = FALSE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){ // will have a for loop

	VS_OUTPUT output;

	// output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );

	// output.normal = normalize(mul(input.normal, (float3x3)gWorld));

	float4 inputPos = float4(input.pos, 1.0f);
	float3 inputNor = input.normal;




	float4 transPos = float4(0, 0, 0, 0);
	float3 transNor = float3(0, 0, 0);

	//one vertex can be attachted/skinned to up to 4 bones (we can add 4 indices and weights per vertex)
	//so, we goot to loop all possivel bones
    for (int pos = 0; pos < 4; ++pos)
    {
        int index = input.blendIndices[pos];

        if(index >= 0)
        {
			float4 bonePos = mul(inputPos, gBones[index]); //Calculate BoneSpace Position
			transPos += mul(input.blendWeights[pos], bonePos); //Append this Position to the transPos &  based on the corresponding bone weigh

			float3 boneNor = mul(inputNor, (float3x3) gBones[index]); //Calculate BoneSpace Normal (only the rotation, never scale or translation for normals)
			transNor += mul(input.blendWeights[pos], boneNor); ////Append this Normla to the transNor &  based on the corresponding bone weigh
        }
    }

	// Step 1:	convert position into float4 and multiply with matWorldViewProj
	output.pos = mul(transPos, gWorldViewProj); 
		// Step 2:	rotate the normal: NO TRANSLATION
	//			this is achieved by clipping the 4x4 to a 3x3 matrix, 
	//			thus removing the postion row of the matrix
    output.normal = normalize(mul(transNor, (float3x3) gWorld)); 

	output.texCoord = input.texCoord;
	return output;


}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb , color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		//it has to be in this order in order for it to work... why? idk
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetRasterizerState(Solid);
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
	}
}

