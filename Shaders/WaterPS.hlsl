Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);
Texture2D tex2 : register(t2);
SamplerState samplerLinear : register(s0);
cbuffer waveBuffer : register(b0)
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

float3 RNMCombine(float3 n1, float3 n2)
{
    float3 t = float3(n1.xy + n2.xy, n1.z * n2.z - dot(n1.xy, n2.xy));
    return normalize(t);
}

PS_OUTPUT PS(PS_INPUT input)
{
    PS_OUTPUT output;
    
    //float4 colour = tex0.Sample(samplerLinear, input.TexCoords);
    float4 colour = input.Colour;
    input.TexCoords += float2(time * 0.005f, time * 0.01f);
    float4 normalSample = tex1.Sample(samplerLinear, input.TexCoords);
    float3 normalCurr;
    input.Normal = normalize(input.Normal);
    normalCurr = RNMCombine(normalSample.xyz, input.Normal);
    output.Colour = float4(colour.rgb, 0.08);
    output.Normal = float4(normalize(normalCurr), 0.533);
    output.Depth = input.ViewPos.z;
    return output;
}
