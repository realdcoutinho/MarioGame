//************
// VARIABLES *
//************
float4x4 m_MatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 m_MatrixWorld : WORLD;
float3 m_LightDirection : DIRECTION
<
	string Object="TargetLight";
> = float3(0.577f, -0.577f, 0.577f);

float3 gColorDiffuse : COLOR = float3(1,1,1);

float gSpikeLength
<
	string UIWidget="Slider";
	float UIMin=0.0f;
	float UIMax=0.5f;
	float UIStep=0.0001f;
> = 0.2f;

RasterizerState FrontCulling 
{ 
	CullMode = FRONT; 
};

//**********
// STRUCTS *
//**********
struct VS_DATA
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
	float4 color : COLOR0;
    float3 Normal : NORMAL;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
	////Step 1.
	////Delete this transformation code and just return the VS_DATA parameter (vsData)
	////Don't forget to change the return type!

 //   GS_DATA temp = (GS_DATA) 0;
 //   temp.Position = mul(float4(vsData.Position, 1), m_MatrixWorldViewProj);
 //   temp.Normal = mul(vsData.Normal, (float3x3) m_MatrixWorld);

 //   return temp;
    return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal) //delete float2 texCoord
{
	//Step 1. Create a GS_DATA object
	GS_DATA temp = (GS_DATA) 0;
	temp.Position = float4(pos, 1.0f);
	temp.Normal = normal;
	
	//Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA object).Position (Keep in mind: float3 -> float4)
	temp.Position = mul(temp.Position, m_MatrixWorldViewProj);
	
	//Step 3. Transform the normal using the World Matrix and assign it to (GS_DATA object).Normal (Only Rotation, No translation!)
	temp.Normal = mul(temp.Normal, (float3x3) m_MatrixWorld);
	
	//Step 4. Append (GS_DATA object) to the TriangleStream parameter (TriangleStream::Append(...))
	triStream.Append(temp);
	
	
	//TriangleStream::Append(temp);
}

[maxvertexcount(9)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	//Use these variable names
    float3 basePoint, top, left, right, spikeNormal;

	//Step 1. Calculate CENTER_POINT
	float3 centerPoint;
	float3 v1 = vertices[0].Position;
	float3 v2 = vertices[1].Position;
	float3 v3 = vertices[2].Position;
	
	centerPoint = (v1 + v2 + v3) / 3;
	


	//Step 2. Calculate Face Normal (Original Triangle)
	////for vertex v0
	//float3 v0e0 = v2 - v1;
	//float3 v0e1 = v3 - v1;
	//float3 v0n = normalize(cross(v0e0, v0e1));
	//float3 normalC = mul(float4(v0n, 0.0f), (float3x3) m_MatrixWorld);
	//normalC = normalize(normalC);
	//float3 normalC = v0n;

	float3 n1 = vertices[0].Normal;
	float3 n2 = vertices[1].Normal;
	float3 n3 = vertices[2].Normal;
	float3 normalC = (n1 + n2 + n3) / 3;
	normalC = normalize(normalC);
	

	//Step 3. Offset CENTER_POINT (use gSpikeLength)
	centerPoint = centerPoint + (gSpikeLength * normalC);
	
	//Step 4 + 5. Calculate Individual Face Normals (Cross Product of Face Edges) & Create Vertices for every face
	float3 e0;
	float3 e1;
	float3 n;
	float3 normal;
	




        //FACE 1
		top = centerPoint;
		left = v3;
		right = v1;
		
		e0 = top - left;
		e1 = right - left;
		n = normalize(cross(e0, e1));
		//normal = mul(float4(n, 0.0f), (float3x3)m_MatrixWorld);
		//spikeNormal = normalize(n);

	
        //faceNormal1 = ...
        CreateVertex(triStream, top, n);
		CreateVertex(triStream, left, n);
		CreateVertex(triStream, right, n);
		
		triStream.RestartStrip();

        //Restart Strip! >> We want to start a new triangle (= interrupt trianglestrip)
        //TriangleStream::RestartStrip();

        //FACE 2
		top = centerPoint;
		left = v1;
		right = v2;
		
		e0 = top - left;
		e1 = right - left;
		n = normalize(cross(e0, e1));
		//normal = mul(float4(n, 0.0f), (float3x3)m_MatrixWorld);
		//spikeNormal = normalize(n);
		
        //faceNormal1 = ...
        CreateVertex(triStream, top, n);
		CreateVertex(triStream, left, n);
		CreateVertex(triStream, right, n);

		triStream.RestartStrip();
		
		
		//Face 3
		top = centerPoint;
		left = v2;
		right = v3;
		
		e0 = top - left;
		e1 = right - left;
		n = normalize(cross(e0, e1));
		//normal = mul(float4(n, 0.0f), (float3x3)m_MatrixWorld);
		//spikeNormal = normalize(n);

        //faceNormal1 = ...
        CreateVertex(triStream, top, n);
		CreateVertex(triStream, left, n);
		CreateVertex(triStream, right, n);
		
		triStream.RestartStrip();
        //...

    //Step 6. Complete code in CreateVertex(...)
	
	
    //Step 7. Bind this Geometry Shader function to the effect pass (See Technique Struct)
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    input.Normal=normalize(input.Normal);
	float diffuseStrength = max(dot(normalize(m_LightDirection),-input.Normal),0.2f); 
	return float4(gColorDiffuse*diffuseStrength,1.0f);
}

//*************
// TECHNIQUES *
//*************
technique10 Default
{
    pass p0
    {
        SetRasterizerState(FrontCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, SpikeGenerator()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}