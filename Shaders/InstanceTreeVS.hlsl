//this is tree vertex shader
cbuffer staticMeshBuffer : register(b0)
{
    float4x4 View;
    float4x4 Proj;
};
cbuffer timeBuffer : register(b1)
{
    float time;
};
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
    float4x4 World : WORLD;
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
    if(input.Colour.g == 1.0)
    {
        //input.Pos.x can make it not same for all instances
        input.Pos.y += sin(time + input.Pos.x * 5.0) * 5 * input.TexCoords.x;
    }

    output.Pos = mul(input.Pos, input.World);
    output.Pos = mul(output.Pos, View);
    float4 viewPos = output.Pos;
    output.Pos = mul(output.Pos, Proj);
    
    output.Normal = mul(input.Normal, (float3x3)input.World);
    output.Tangent = mul(input.Tangent, (float3x3)input.World);
    output.TexCoords = input.TexCoords;
    output.Colour = input.Colour;
    output.WorldPos = output.Pos.xyz;
    output.ViewPos = viewPos.xyz;
    return output;
}