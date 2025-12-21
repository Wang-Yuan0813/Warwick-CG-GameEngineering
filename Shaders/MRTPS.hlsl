Texture2D tex0 : register(t0); //albedo + roughness (3 + 1)
Texture2D tex1 : register(t1); //normal + Freshnel (3 + 1), if freshel == 1, means this object is self-illuminating
Texture2D<float> tex2 : register(t2); //depth
SamplerState samplerLinear : register(s0);
cbuffer cameraBuffer : register(b0)
{
    float4x4 gInvProj;
    float4x4 gInvView;
    float3 gCameraPosWS;
};
cbuffer lightBuffer : register(b1)
{
    float time;
    float3 gLightPosWS;
    float gLightRadius;
    float3 gLightColor;
    float gLightIntensity;
    
    int gNumLights;
    float4 gLightPosWSArray[10];
    /*float gLightRadiusArray[10];
    float3 gLightColorArray[10];
    float gLightIntensityArray[10];*/
};
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Texcoords : TEXCOORD;
};

float3 ReconstructViewRay(float2 uv)
{
    float2 ndc = float2(uv.x * 2.0 - 1.0, (1.0 - uv.y) * 2.0 - 1.0);
    float4 clip = float4(ndc, 1.0, 1.0);
    float4 v = mul(clip, gInvProj);
    v /= v.w;
    return v.xyz;
}

float3 ReconstructViewPos(float2 uv, float viewZ)
{
    float3 ray = ReconstructViewRay(uv);
    float z = max(abs(ray.z), 1e-6);
    return ray * (viewZ / z);
}

//Lambert + Blinn-Phong
float smoothAttenuation(float dist, float radius)
{
    float invSq = 1.0 / max(dist * dist, 1e-4);
    float fade = saturate(1.0 - dist / max(radius, 1e-4));
    return invSq * fade;
}

float smoothAttenuationBetter(float dist, float radius)
{
    float r = max(radius, 1e-4);
    float x = saturate(1.0 - dist / r); // 0..1
    float smooth = x * (3.0 - 2.0 * x);
    float invSq = 1.0 / max(dist * dist, 1.0);
    return smooth * invSq;
}
float LinearizeDepth_D3D(float depth01, float nearZ, float farZ)
{
    return (nearZ * farZ) / (farZ - depth01 * (farZ - nearZ));
}
float GetLinearDepthGray(float depth01)
{
    // Convert depth from [0,1] to linear depth in world units
    //far and near are hardcoded here, should be same as in projection matrix, 0.1 and 1000.0
    float d_linear = LinearizeDepth_D3D(depth01, 0.1f, 1000.0f);
    return saturate(d_linear / 1000.0f);
}

float3 FresnelSchlick(float3 F0, float VoH)
{
    return F0 + (1.0 - F0) * pow(1.0 - VoH, 5.0);
}
float3 F0FromIOR(float IOR)
{
    float f = (IOR - 1.0) / (IOR + 1.0);
    float F0_scalar = f * f;
    return float3(F0_scalar, F0_scalar, F0_scalar);
}

float3 EvaluatePointLight(
    float3 pWS,
    float3 nWS,
    float3 Vdir,
    float3 albedo,
    float roughness,
    float gIOR, 
    float3 _gLightPosWS,
    float _gLightRadius,
    float3 _gLightColor,
    float _gLightIntensity) 
{
    float3 L = _gLightPosWS - pWS;
    float dist = length(L);
    float3 Ldir = L / max(dist, 1e-4);
    float atten = smoothAttenuationBetter(dist, _gLightRadius);

    float NdotL = saturate(dot(nWS, Ldir));
    float3 diffuse = albedo * NdotL;

    float3 H = normalize(Ldir + Vdir);
    float NdotH = saturate(dot(nWS, H));
    float VoH = saturate(dot(Vdir, H));

    float3 F0 = F0FromIOR(gIOR);
    float3 F = FresnelSchlick(F0, VoH);

    float specPower = lerp(2048.0, 8.0, roughness);
    float specTerm = pow(NdotH, specPower);
    float3 specular = F * specTerm;

    float Favg = (F.x + F.y + F.z) / 3.0;
    diffuse *= (1.0 - Favg);

    float3 lightRGB = _gLightColor * _gLightIntensity;
    return (diffuse + specular) * lightRGB * atten;
}

float4 PS(PS_INPUT input) : SV_Target0
{
    //roughness, albedo, normal, linear_depth
    float4 tex0Sample = tex0.Sample(samplerLinear, input.Texcoords);
    float4 tex1Sample = tex1.Sample(samplerLinear, input.Texcoords);
    float tex2Sample = tex2.Sample(samplerLinear, input.Texcoords);

    
    float3 albedo = tex0Sample.rgb;
    float roughness = saturate(tex0Sample.a);
    
    if (tex1Sample.a == 1) //self-illuminating
        return float4(albedo, 1.0);
    float gIOR = 10 * tex1Sample.a;
    
    float3 nWS = normalize(tex1Sample.xyz);
    float eyeDepth = tex2Sample;
    float3 pVS = ReconstructViewPos(input.Texcoords, eyeDepth);
    float3 pWS = mul(float4(pVS, 1.0f), gInvView).xyz;
    float3 Vdir = normalize(gCameraPosWS - pWS);

    
    float3 lightingAccum = float3(0, 0, 0);
    lightingAccum += EvaluatePointLight(
            pWS, nWS, Vdir, albedo, roughness, gIOR,
            gLightPosWS,
            gLightRadius,
            gLightColor,
            gLightIntensity);
    [unroll] 
    for (int i = 0; i < gNumLights; ++i)
    {
        lightingAccum += EvaluatePointLight(
            pWS, nWS, Vdir, albedo, roughness, gIOR,
            gLightPosWSArray[i].xyz,
            gLightRadius - 50,
            gLightColor,
            gLightIntensity - 50);
    }
    
    float3 finalRGB = lightingAccum + albedo * 0.2;
    //return float4(albedo, 1.0);
    return float4(finalRGB, 1.0);

    /*
    float eyeDepth = tex2Sample;
    float3 nWS = normalize(tex1Sample.xyz);
    float3 pVS = ReconstructViewPos(input.Texcoords, eyeDepth);
    float3 pWS = mul(float4(pVS, 1.0f), gInvView).xyz;
    float3 L = gLightPosWS - pWS;
    float dist = length(L);
    float3 Ldir = L / max(dist, 1e-4);
    float atten = smoothAttenuationBetter(dist, gLightRadius);
    
    float NdotL = saturate(dot(nWS, Ldir));
    float3 diffuse = albedo * NdotL;
    
    float3 Vdir = normalize(gCameraPosWS - pWS);
    float3 H = normalize(Ldir + Vdir);
    float NdotH = saturate(dot(nWS, H));
    float VoH = saturate(dot(Vdir, H));
    
    
    float3 F0 = F0FromIOR(gIOR);
    float3 F = FresnelSchlick(F0, VoH);

    
    float specPower = lerp(2048.0, 8.0, roughness);
    float specTerm = pow(NdotH, specPower);
    float3 specular = F * specTerm;
    
    float Favg = (F.x + F.y + F.z) / 3.0;
    diffuse *= (1.0 - Favg);

    float3 lightRGB = gLightColor * gLightIntensity;
    float3 lit = (diffuse + specular) * lightRGB * atten;
    //return float4(albedo, 1);
    return float4(lit + albedo * 0.2, 1);*/
}