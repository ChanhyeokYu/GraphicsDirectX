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

cbuffer CBMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;  
}

PS_IN VSMain(VS_IN input)
{
    PS_IN output;
    float4 worldPos = mul(float4(input.pos, 1.0f), world);
    float4 viewPos = mul(worldPos, view);
    output.pos = mul(viewPos, projection);
    output.uv = input.uv;
    return output;
};

//Pixel Shader
// 최종 색상 결정
float4 PSMain(PS_IN input) : SV_TARGET
{
    return tex0.Sample(samliner, input.uv);
};