//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
    //Enable Depth Rendering
    DepthEnable = TRUE;
    //Disable Depth Writing
    DepthWriteMask = ZERO;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
    CullMode = BACK;
};


//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    // Set the Position
    output.Position = float4(input.Position, 1.f);
    // Set the TexCoord
    output.TexCoord = input.TexCoord;

    return output;
}


float2 distortUV(float2 uv, float2 offset)
{
    float2 r = uv - 0.5;
    float2 offsetR = offset * r;
    float2 distUV = uv + offsetR;
    return distUV;
}

//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
    float2 redOffset = float2(0.013f, 0.013f);
    float2 blueOffset = float2(-0.013f, -0.013f);


    float4 redColor = gTexture.Sample(samPoint, input.TexCoord + redOffset);
    float4 greenColor = gTexture.Sample(samPoint, input.TexCoord);
    float4 blueColor = gTexture.Sample(samPoint, input.TexCoord + blueOffset);

    return float4(redColor.r, greenColor.g, blueColor.b, 1);
}


//TECHNIQUE
//---------
technique11 ChromaticAberration
{
    pass P0
    {
        // Set states...
        SetRasterizerState(BackCulling);
        SetDepthStencilState(EnableDepth, 0);

        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}
