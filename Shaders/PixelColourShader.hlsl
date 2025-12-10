struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    float4 Colour : COLOUR;
};

float hash21(float2 p)
{
    p = frac(p * float2(123.34, 234.45));
    p += dot(p, p + 34.345);
    return frac(p.x * p.y);
}

float noise2(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);

    float a = hash21(i + float2(0, 0));
    float b = hash21(i + float2(1, 0));
    float c = hash21(i + float2(0, 1));
    float d = hash21(i + float2(1, 1));

    float2 u = f * f * (3.0 - 2.0 * f); 
    return lerp(lerp(a, b, u.x), lerp(c, d, u.x), u.y);
}

float antiAliasFactor(float2 p)
{
    float2 dx = ddx(p);
    float2 dy = ddy(p);
    float footprint = max(length(dx), length(dy));
    return saturate((footprint - 0.25) / (1.5 - 0.25));
}

// Schlick Fresnel
float fresnelSchlick(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float4 PS(PS_INPUT input) : SV_Target0
{
    
    float2 TilingUV = { 8, 8 }; // UV 缩放（例如 8,8），用于无WorldXZ时

    float4 DeepColor = { 0.04, 0.12, 0.6, 1 }; // 深水颜色（例：float4(0.02,0.08,0.15,1)）
    float4 ShallowColor = { 0.05, 0.15, 0.70, 1 }; // 浅水颜色（例：float4(0.05,0.15,0.20,1)）
    float4 FoamColor = { 0.90, 0.95, 1.0, 1 }; // 浪花/漂白色（例：float4(0.90,0.95,1.0,1)）
    
    float FoamAmount = 0.1; // 浪花权重（0~1，例：0.25）
    float Noise1Amp = 0.08; // 低频噪声振幅（例：0.08）
    float Noise2Amp = 0.04; // 次级噪声振幅（例：0.04）
    float FresnelF0 = 0.02; // 基反射率（例：0.02）
    float SpecularPower = 64; // 高光指数（例：64.0）

    float3 SunDir = {1,1,1 }; // 太阳方向（单位向量）
    float SunStrength = 2; // 高光强度（例：2.0）

    float ContrastDamp = 0.7; // 远处对比度抑制（例：0.7）

    
// 1) 选择坐标域：优先用世界XZ，否则用UV
    float2 p = input.TexCoords * TilingUV;
    // 2) 静态颜色基础：深-浅水渐变（用低频 noise 混合）
    float n1 = noise2(p * 0.25); // 更低频（大尺度）0.25-0.75
    float n2 = noise2(p * 1.3); // 次级细节（仍然低频）1.3-3.0

    // 抗混叠：根据像素足迹抑制对比度（避免远处噪声高频引起摩尔纹）
    float aa = antiAliasFactor(p);
    float n1AA = lerp(n1, 0.5, aa * 0.6); // 越远越向中性过渡
    float n2AA = lerp(n2, 0.5, aa * 0.8);

    // 合成静态水色（无波动）
    float blend = saturate(0.4 + (n1AA - 0.5) * 0.6 + (n2AA - 0.5) * 0.3);
    float4 baseWater = lerp(DeepColor, ShallowColor, blend);

    // 3) 浪花/漂白（只做静态点缀，避免过度高频）
    float foamMaskRaw = saturate((n2AA - 0.6) * 4.0); // 提升高值作为浪花
    // 用 fwidth 做 AA（减少边界锯齿）
    float fw = fwidth(foamMaskRaw);
    float foamMask = smoothstep(0.6 - fw, 0.6 + fw, foamMaskRaw);
    baseWater = lerp(baseWater, FoamColor, foamMask * FoamAmount);

    // 4) 简易法线/高光（静态质感，可选）
    float3 N = normalize(input.Normal); // 使用传入法线（模型或世界）
    float3 L = normalize(SunDir);
    float3 V = float3(0, 1, 0); // 视线方向：若有真实视线请传入
    float3 H = normalize(L + V);

    float NdotL = saturate(dot(N, L));
    float NdotH = saturate(dot(N, H));
    float spec = pow(NdotH, SpecularPower) * SunStrength;

    float Fr = fresnelSchlick(saturate(dot(N, V)), FresnelF0);

    float4 litColor = baseWater * (0.2 + 0.5 * NdotL) + spec;
    float4 reflectCol = lerp(litColor, baseWater + 0.15, Fr); // 简化反射，不采样贴图

    // 5) 远处整体对比度抑制（强力杀摩尔纹）
    float2 dx = ddx(p), dy = ddy(p);
    float footprint = max(length(dx), length(dy));
    float4 avg = 0.5 * (DeepColor + ShallowColor);
    float t = saturate((footprint - 0.5) / (2.0 - 0.5));
    float4 finalColor = lerp(reflectCol, avg, t * ContrastDamp);

    return finalColor;

    
    /*float4 finalColor = { abs(normalize(input.Normal)) * 0.9f, 1.0 };
    return finalColor;*/
    
    /*float2 Tiles = { 127, 127 }; 
    float4 ColorA = { 0.95f, 0.95f, 0.95f, 1.0f }; 
    float4 ColorB = { 0.15f, 0.15f, 0.15f, 1.0f };

    
    float2 uvT = input.TexCoords * max(Tiles, float2(1, 1));
    
    
    float parity = fmod(floor(uvT.x) + floor(uvT.y), 2.0);
    float4 baseColor = lerp(ColorA, ColorB, parity);
    
    
    float2 dX = ddx(uvT);
    float2 dY = ddy(uvT);

    float footprint = max(length(dX), length(dY)); 

    float4 avgColor = 0.5 * (ColorA + ColorB);

    float k = 1.2;
    float t = saturate((footprint - 0.25) / (1.5 - 0.25));
    float4 outColor = lerp(baseColor, avgColor, k * t);

    return outColor;*/

    //return float4(input.Colour);
}