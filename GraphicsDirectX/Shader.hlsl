// Vertex Shader
struct VS_IN
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
};

//Pixel Shader
float4 PSMain(PS_IN input) : SV_TARGET
{
    return float4(input.color, 1.0f);
};