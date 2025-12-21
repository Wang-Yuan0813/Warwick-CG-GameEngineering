Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
SamplerState samplerLinear : register(s0);
cbuffer spinBuffer : register(b0)
{
    float time;
};
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
    float3 ViewPos : TEXCOORD1;
};
struct PS_OUTPUT
{
    float4 Colour : SV_Target0;
    float4 Normal : SV_Target1;
    float Depth : SV_Target2;
};
PS_OUTPUT PS(PS_INPUT input)
{
    PS_OUTPUT output;
    input.TexCoords.x += time * 0.005f;
    float4 colour = tex0.Sample(samplerLinear, input.TexCoords);    
    input.Normal = normalize(input.Normal);
    output.Colour = float4(colour.rgb, 0.08);
    output.Normal = float4(input.Normal.xyz, 1);
    output.Depth = input.ViewPos.z;
    return output;
}
