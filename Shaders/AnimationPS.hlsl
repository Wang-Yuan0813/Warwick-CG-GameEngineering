Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
SamplerState samplerLinear : register(s0);
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
    float3 ViewPos : TEXCOORD1;
    float3 WorldPos : WORLDPOS;
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
    
    float4 colour = tex0.Sample(samplerLinear, input.TexCoords);
    float4 normalSample = tex1.Sample(samplerLinear, input.TexCoords);
    float roughness = tex2.Sample(samplerLinear, input.TexCoords).r;
    
    float3 normalCurr = normalize(normalSample.xyz);
    output.Colour = float4(colour.rgb, roughness);
    output.Normal = float4(normalize(normalCurr), 0.133);
    output.Depth = input.ViewPos.z;
    return output;
}
