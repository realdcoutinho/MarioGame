//INCOMPLETE!

float4x4 gTransform : WORLDVIEWPROJECTION;
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

RasterizerState BackCulling
{
	CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	int Channel : TEXCOORD2; //Texture Channel
	float3 Position : POSITION; //Left-Top Character Quad Starting Position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Left-Top Character Texture Coordinate on Texture
	float2 CharSize: TEXCOORD1; //Size of the character (in screenspace)
};

struct GS_DATA
{
	float4 Position : SV_POSITION; //HOMOGENEOUS clipping space position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Texcoord of the vertex
	int Channel : TEXCOORD1; //Channel of the vertex
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{
	//Create a new GS_DATA object
	//Fill in all the fields
	//Append it to the TriangleStream

	GS_DATA geomData = (GS_DATA) 0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.Channel = channel;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//REMOVE THIS >
	//GS_DATA dummyData = (GS_DATA)0; //Just some dummy data
	//triStream.Append(dummyData); //The geometry shader needs to emit something, see what happens if it doesn't emit anything.
	//< STOP REMOVING

	//Create a Quad using the character information of the given vertex
	//Note that the Vertex.CharSize is in screenspace, TextureCoordinates aren't ;) [Range 0 > 1]
	
	float x = gTextureSize.x;
	float y = gTextureSize.y;

	//////////////////////////////////THERE IS A CLIPING FUNCTION FOR THIS *FACEPLAM*
	// float minimum = 0;
	// float maximum = 1;
	// Transform x to y in the range [0,1]
	// float newX = (x - minimum) / (maximum - minimum);
	// float newY = (y - minimum) / (maximum - minimum);

    clip(x < 0.1f ? 0 : 1);
	clip(y < 0.1f ? 0 : 1);



	float2 charSize;
    charSize.x = vertex[0].CharSize.x; 
    charSize.y = vertex[0].CharSize.y;

	float3 pos = vertex[0].Position;
	float4 color = vertex[0].Color;
	float2 texCoord = vertex[0].TexCoord;
	int channel = vertex[0].Channel;


    // LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
    // |          / |
    // |       /    |
    // |    /       |
    // | /          |
    // LB----------RB

    

	//1. Vertex Left-Top [0, 0]
	////position calculation 
	/////not needed. its the the original position [0, 0]
	CreateVertex(triStream, pos, color, texCoord, channel);

	//2. Vertex Right-Top [1, 0]
	////pos caculation
	pos.x += charSize.x; //the position moves only to the right [0+1, 0]
	////texture calculation //
	texCoord.x += charSize.x / gTextureSize.x;
	CreateVertex(triStream, pos, color, texCoord, channel);

	//3. Vertex Left-Bottom [0, 1]
	////pos calculation //the position moves only to the bottom [1-1, 1], but I already changed the x text coor pos. change it back
	pos.x -= charSize.x; //[1-1]
	pos.y += charSize.y; //[0+1]
	////texture calculation
	texCoord.x -= charSize.x / gTextureSize.x;
	texCoord.y += charSize.y / gTextureSize.y;
	CreateVertex(triStream, pos, color, texCoord, channel);

	//4. Vertex Right-Bottom [1,1]
	////position calculation // the position moves both to the right and to the bottom. the position is already correct on y, so only change on x
	pos.x += charSize.x; //[1+1, 1]
	//texture calculation
	texCoord.x += charSize.x / gTextureSize.x;
	CreateVertex(triStream, pos, color, texCoord, channel);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET{

	//Sample the texture and return the correct channel [Vertex.Channel]
	//You can iterate a float4 just like an array, using the index operator
	//Also, don't forget to colorize ;) [Vertex.Color]


    float4 color = gSpriteTexture.Sample(samPoint, input.TexCoord)[input.Channel];

    color *= input.Color;

    return color;

	//return input.Color; //TEMP
}

// Default Technique
technique10 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
