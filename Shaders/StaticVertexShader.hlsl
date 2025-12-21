cbuffer staticMeshBuffer : register(b0)
{
    float4x4 W;
    float4x4 View;
    float4x4 Proj;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
    float3 WorldPos : WORLDPOS;
    float3 ViewPos : TEXCOORD1;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(input.Pos, W);
    output.Pos = mul(output.Pos, View);
    float4 viewPos = output.Pos;
    
    output.Pos = mul(output.Pos, Proj);
    output.Normal = mul(input.Normal, W);
    output.Tangent = mul(input.Tangent, W);
    output.TexCoords = input.TexCoords;
    output.Colour = input.Colour;
    output.ViewPos = viewPos.xyz;
    output.WorldPos = W[3].xyz;
    return output;
    /*PS_INPUT output;
    output.Pos = mul(input.Pos, W);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Proj);
    output.Normal = mul(input.Normal, (float3x3)W);
    output.Tangent = mul(input.Tangent, (float3x3)W);
    output.TexCoords = input.TexCoords;
    return output;*/
}