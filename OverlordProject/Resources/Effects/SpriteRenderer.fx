//INCOMPLETE!

float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};


//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0;
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
    //Step 3.
    if (rotation == 0)
    {
        //Step 3.1
        //Apply pivot offset
        pos.x -= pivotOffset.x;
        pos.y -= pivotOffset.y;
    }
    else
    {
        //3.1
        //Transform position to origin (based on the offsets
        pos.x -= pivotOffset.x;
        pos.y -= pivotOffset.y;
        //////FIX
        pos.x -= offset.x;
        pos.y -= offset.y;
        
        //3.2
        //Rotate the position 
        float2 givenPosition = pos;
        pos.x = (givenPosition.x * rotCosSin.x) - (givenPosition.y * rotCosSin.y);
        pos.y = (givenPosition.y * rotCosSin.x) + (givenPosition.x * rotCosSin.y);

        //3.3
        //ReTransform position using the initial offset
        ////FIX
        pos.x += offset.x;
        pos.y += offset.y;


        //IMPORTANT. WITHOUT THESE SUBTRACT AND LATER ADDITON OF THE OFFSET, THE SPRITE WOULD ROTATE AROUND THE ORIGIN AND NOT ITSELF
    }

    //Geometry Vertex Output
    GS_DATA geomData = (GS_DATA)0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
    //Given Data (Vertex Data)
    float3 position = float3(vertex[0].TransformData.x, vertex[0].TransformData.y, vertex[0].TransformData.z); //Extract the position data from the VS_DATA vertex struct
    float2 offset = float2(vertex[0].TransformData.x, vertex[0].TransformData.y); //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float rotation = vertex[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
    float2 pivot = float2(vertex[0].TransformData2.x, vertex[0].TransformData2.y); //Extract the pivot data from the VS_DATA vertex struct
    float2 scale = float2(vertex[0].TransformData2.z, vertex[0].TransformData2.w); //Extract the scale data from the VS_DATA vertex struct



    position.x += (gTextureSize.x * scale.x);
    position.y += (gTextureSize.y * scale.y);

    // LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
    // |          / |
    // |       /    |
    // |    /       |
    // | /          |
    // LB----------RB


    float2 rotCosSin = (0, 0);

    if(rotation == 0)
    {
        rotCosSin = float2(0, 0);
    }
    else 
    {
        rotCosSin.x = cos(rotation);
        rotCosSin.y = sin(rotation);
    }


    float2 pivotOffSet = float2(pivot.x * gTextureSize.x * scale.x, pivot.y * gTextureSize.y * scale.y); //calculations for pivot offset //STEP 2
    float2 texCoord = float2(0, 0); //Initial Texture Coordinate
    float4 color = vertex[0].Color;


    //VERTEX 1 [LT]
    texCoord = float2(0, 0); //Step1
    position.x = vertex[0].TransformData.x; //Step1
    position.y = vertex[0].TransformData.y; //Step1
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffSet); //Change the color data too!

    //VERTEX 2 [RT]
    texCoord = float2(1, 0); //Step1
    position.x += (gTextureSize.x * scale.x); //Step1
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffSet); //Change the color data too!

    //VERTEX 3 [LB]
    texCoord = float2(0, 1); //Step1
    position.x = vertex[0].TransformData.x; //Step1
    position.y += (gTextureSize.y * scale.y); //Step1
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffSet); //Change the color data too!

    //VERTEX 4 [RB]
    texCoord = float2(1, 1); //Step1
    position.x += (gTextureSize.x * scale.x); //Step1
    CreateVertex(triStream, position, color, texCoord, rotation, rotCosSin, offset, pivotOffSet); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{

    //return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
    float4 sample = gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;
   // clip(sample.a < 0.1f ? -1 : 1);
    return sample;
}

// Default Technique
technique11 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}
