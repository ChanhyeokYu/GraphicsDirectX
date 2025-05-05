Texture2D tex0 : register(t0);
SamplerState samliner : register(s0);

cbuffer TransformBuffer : register(b0)
{
    matrix worldViewProj;
};

// Vertex Shader
// 정점 위치 계산
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output;
    output.pos = mul(float4(input.pos, 1.0f), worldViewProj);
    output.uv = input.uv;
    return output;
};

//Pixel Shader
// 최종 색상 결정
float4 PSMain(PS_IN input) : SV_TARGET
{
    return tex0.Sample(samliner, input.uv);
};