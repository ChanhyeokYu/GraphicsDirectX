Texture2D tex0 : register(t0);
SamplerState samlinear : register(s0);
//cbuffer TransformBuffer : register(b0)
//{
//    matrix worldViewProj;
//};


// Vertex Shader
// 정점 위치 계산
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

//struct PS_IN
//{
//    float4 pos : SV_POSITION;
//    float2 uv : TEXCOORD;
//};

cbuffer CBMatrix : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;  
}

VS_OUT VSMain(VS_IN input)
{
    VS_OUT output;
    float4 worldPos = mul(float4(input.pos, 1.0f), world);
    output.pos = mul(view, worldPos);
    output.pos = mul(projection, output.pos);
    output.uv = input.uv;
    
    return output;
};

//Pixel Shader
// 최종 색상 결정
float4 PSMain(VS_OUT input) : SV_TARGET
{
     //return float4(1, 0, 0, 1); // 무조건 빨간색
    return tex0.Sample(samlinear, input.uv);
};