#include "Global.hlsli"

struct VS_INPUT
{
    float2 in_pos : POSITION;
};

struct VS_OUTPUT
{
    float4 sv_Position : SV_POSITION;
    float2 uv0 : UV0;
};

BEGIN_PUSH_CONSTANTS()
    PUSH_CONSTANTS(element_id, int, 0)
    PUSH_CONSTANTS(flag1, int, 4)
    PUSH_CONSTANTS(flag2, int, 8)
    PUSH_CONSTANTS(flag3, int, 12)
END_PUSH_CONSTANTS()

struct DrawElementData
{
    float4 color;
    float2 scale;
    float2 translate;
};

STRUCTURE_BUFFER(DrawElementData, ElementDataBuffer, ENTITY, 0);

float4 GetElementColor()
{
    return ElementDataBuffer[PUSH_CONSTANTS.element_id].color;
}

float2 GetElementScale()
{
    return ElementDataBuffer[PUSH_CONSTANTS.element_id].scale;
}

float2 GetElementTranslate()
{
    return ElementDataBuffer[PUSH_CONSTANTS.element_id].translate;
}

VS_OUTPUT BoxVS(VS_INPUT input)
{
    VS_OUTPUT output;
    float2 new_position = input.in_pos * GetElementScale() + GetElementTranslate();

    output.sv_Position = float4(new_position * 2 - 1, 0, 1);
    output.sv_Position.y = -output.sv_Position.y;
    output.uv0 = input.in_pos;
    return output;
}

float4 BoxPS(VS_OUTPUT input) : SV_TARGET
{
    return GetElementColor();
}

TEXTURE2D(image, PS);
float4 BoxPSWithImage(VS_OUTPUT input) : SV_TARGET
{
    SamplerState defaultSampler;
    return image.Sample(defaultSampler, input.uv0);  
}