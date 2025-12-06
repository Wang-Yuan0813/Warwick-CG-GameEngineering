cbuffer animatedMeshBuffer
{
    float4x4 W;
    float4x4 VP;
    float4x4 bones[256];
};
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    uint4 BoneIDs : BONEIDS;
    float4 BoneWeights : BONEWEIGHTS;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    float4x4 transform = bones[input.BoneIDs[0]] * input.BoneWeights[0];
    transform += bones[input.BoneIDs[1]] * input.BoneWeights[1];
    transform += bones[input.BoneIDs[2]] * input.BoneWeights[2];
    transform += bones[input.BoneIDs[3]] * input.BoneWeights[3];
    
    output.Pos = mul(input.Pos, transform);
    //output.Pos = input.Pos;
    
    output.Pos = mul(output.Pos, W);
    output.Pos = mul(output.Pos, VP);
    
    output.Normal = mul(input.Normal, (float3x3) transform);
    //output.Normal = input.Normal;
    
    output.Normal = mul(output.Normal, (float3x3) W);
    output.Normal = normalize(output.Normal);
    
    output.Tangent = mul(input.Tangent, (float3x3) transform);
    //output.Tangent = input.Tangent;
    
    output.Tangent = mul(output.Tangent, (float3x3) W);
    output.Tangent = normalize(output.Tangent);
    output.TexCoords = input.TexCoords;
    return output;
}
