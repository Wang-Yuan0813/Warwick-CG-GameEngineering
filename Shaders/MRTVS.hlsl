struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Texcoords : TEXCOORD;
};
PS_INPUT VS(uint vertexID : SV_VertexId)
{
    PS_INPUT output;
    output.Texcoords = float2((vertexID << 1) & 2, vertexID & 2);
    output.Pos = float4((output.Texcoords.x * 2.0f) - 1.0f, (-output.Texcoords.y * 2.0f) + 1.0f, 0, 1.0f);
    return output;
}
