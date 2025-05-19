#include "Global.hlsli"

struct VSInput
{
    float2 position : POSITION;
    float2 uv : UV0;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV0;
};

BEGIN_PUSH_CONSTANTS()
    PUSH_CONSTANTS(scale, float2, 0)
    PUSH_CONSTANTS(translate, float2, 8)
END_PUSH_CONSTANTS()

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.color = input.color;
    output.uv = input.uv;
    output.position = float4(input.position * PUSH_CONSTANTS.scale + PUSH_CONSTANTS.translate, 0, 1);
    return output;
}

TEXTURE2D(sTexture, PS);

float4 PSMain(VSOutput input) : SV_Target
{
    SamplerState defaultSampler;
    return input.color * sTexture.Sample(defaultSampler, input.uv).rgba;
}