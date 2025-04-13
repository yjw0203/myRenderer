#include "Global.hlsli"

struct MeshVertexOutput
{
    float4 sv_Position : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : UV0;
};

struct VS_INPUT
{
    float2 in_pos : POSITION;
};

struct VS_OUTPUT
{
    float4 sv_Position : SV_POSITION;
    float2 pos : UV0;
};

VS_OUTPUT PostProcessVS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.sv_Position = float4(input.in_pos, 1.0f, 1.0f);
    output.pos = input.in_pos;
    return output;
}

TEXTURE2D(depthTex, PS);
TEXTURE2D(normalTex, PS);

float LinearizeDepth(float depth) {
    return (2.0 * near_far.x) / (near_far.y + near_far.x - depth * (near_far.y - near_far.x));
}


float4 PostProcessPS(VS_OUTPUT input) : SV_TARGET
{
    SamplerState defaultSampler;
    float2 uv = (input.pos.xy + float2(1.0, 1.0)) / 2;
    float depth = depthTex.Sample(defaultSampler, uv).r;
    float3 normal = normalTex.Sample(defaultSampler, uv).rgb;

    float2 texelSize = float2(1.0 / 600.f, 1.0 / 600.f);
    if (screenSize.y == 0)
    {
        texelSize = float2(1.0 / 601.f, 1.0 / 601.f);
    }
    
    float centerDepth = LinearizeDepth(depth);
    float leftDepth = LinearizeDepth(depthTex.Sample(defaultSampler, uv - float2(texelSize.x, 0.0)).r);
    float topDepth = LinearizeDepth(depthTex.Sample(defaultSampler, uv - float2(0.0, texelSize.y)).r);
    float edge = abs(leftDepth - centerDepth) + abs(topDepth - centerDepth);

    return float4(1.0, 1.0, 0.0, edge * 0.5);
}

float4 Mask(MeshVertexOutput ps_in) : SV_TARGET
{
    return float4(ps_in.normal,1);
}