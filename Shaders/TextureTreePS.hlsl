Texture2D tex0 : register(t0); //albedo
Texture2D tex1 : register(t1); //normal
Texture2D tex2 : register(t2); //roughness
SamplerState samplerLinear : register(s0);
struct PS_OUTPUT
{
    float4 Colour : SV_Target0;
    float4 Normal : SV_Target1;
    float Depth : SV_Target2;
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
float3 UnpackNormal(float3 n01)
{
    return normalize(n01 * 2.0f - 1.0f);
}
PS_OUTPUT PS(PS_INPUT input)
{
    float4 albedoSample = tex0.Sample(samplerLinear, input.TexCoords);
    if (albedoSample.a < 0.1f)
        discard;
    
    float3 n_ts = UnpackNormal(tex1.Sample(samplerLinear, input.TexCoords).xyz);
    
    float3 N_ws = normalize(input.Normal);
    float3 T_ws = normalize(input.Tangent);
    float3 B_ws = normalize(cross(N_ws, T_ws));
    float3x3 TBN = float3x3(T_ws, B_ws, N_ws);
    float3 n_ws = normalize(mul(n_ts, TBN));
    
    
    PS_OUTPUT output;
    float freshness = 1.55f; 
    output.Colour = albedoSample;   
    output.Normal = float4(n_ws, freshness / 10);
    float roughness = tex2.Sample(samplerLinear, input.TexCoords).r;
    roughness = saturate(roughness);
    output.Colour.a = roughness; //store roughness in alpha channel of colour output
    output.Depth = input.ViewPos.z;
    return output;
}
/*Texture2D tex0 : register(t0);  //albedo
Texture2D tex1 : register(t1); //normal
Texture2D tex2 : register(t2); //roughness
SamplerState samplerLinear : register(s0);

float3 DecodeNormalTS(float3 nTS, float normalScale)
{
    float3 n = nTS * 2.0f - 1.0f;
    n.xy *= normalScale;
    return normalize(n);
}

float3 TangentToWorld(float3 nTS, float3 N_ws, float3 T_ws)
{
    float3 N = normalize(N_ws);
    float3 T = normalize(T_ws);
    float3 B = normalize(cross(N, T)); 
    float3x3 TBN = float3x3(T, B, N);
    return normalize(mul(nTS, TBN));
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = max(roughness, 0.001f);
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;
    float denom = 3.14159265f * pow((NdotH2 * (a2 - 1.0f) + 1.0f), 2.0f);
    return a2 / max(denom, 1e-6f);
}

float GeometrySchlickGGX(float NdotX, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) * 0.125f; // 1/8
    return NdotX / (NdotX * (1.0f - k) + k);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

float3 ComputeF0(float3 albedo, float metalness)
{
    float3 F0_dielectric = float3(0.04, 0.04, 0.04);
    return lerp(F0_dielectric, albedo, saturate(metalness));
}
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
    float4 WorldPos : WORLDPOS;
};
float4 PS(PS_INPUT input) : SV_Target0
{
    //simple lighting
    float3 lightDir = normalize(float3(0, 1.0, 0));
    
    //albedo sampling
    float4 albedoSample = tex0.Sample(samplerLinear, input.TexCoords);
    if (albedoSample.a < 0.1f)
        discard; 

    float3 albedo = albedoSample.rgb;

    //normal sampling
    float gNormalScale = 1.0f;
    float3 nTS = tex1.Sample(samplerLinear, input.TexCoords).rgb;
    float3 N = TangentToWorld(DecodeNormalTS(nTS, gNormalScale),
                                 input.Normal, input.Tangent);

    //roughness sampling
    float roughness = tex2.Sample(samplerLinear, input.TexCoords).r;
    roughness = saturate(roughness);

    //metalness
    float gMetalness = 0.0f;
    
    
    float3 V = normalize(lightDir - input.WorldPos.xyz);
    float3 L = normalize(lightDir); 
    float3 H = normalize(V + L);

    float3 F0 = ComputeF0(albedo, gMetalness);
    float D = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

    float NdotL = max(dot(N, L), 0.0f);
    float NdotV = max(dot(N, V), 0.0f);
    float3 spec = (D * G * F) / max(4.0f * NdotL * NdotV, 1e-5f);

    //diffuse
    float3 kd = (1.0f - F) * (1.0f - gMetalness);
    float3 diffuse = kd * albedo / 3.14159265f;

    // light radiance
    float3 radiance = float3(1.0f, 1.0f, 1.0f) * 1;

    //light contribution
    float3 Lo = (diffuse + spec) * radiance * NdotL;

    //environment ambient
    float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

    float3 color = ambient + Lo;

    //color tone mapping + gamma correction
    color = color / (color + 1.0f); //simple Reinhard
    color = pow(color, 1.0f / 2.2f); // Gamma

    return float4(color, 1.0f);
}*/
