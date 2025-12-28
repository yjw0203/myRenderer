#include "Global.hlsli"

struct VS_INPUT
{
    float2 in_pos : POSITION;
    int instance_id : SV_InstanceID;
};

struct VS_OUTPUT
{
    float4 sv_Position : SV_POSITION;
    float4 color : COLOR;
    float2 uv0 : UV0;
};

BEGIN_PUSH_CONSTANTS()
    PUSH_CONSTANTS(flag0, int, 0)
    PUSH_CONSTANTS(flag1, int, 4)
    PUSH_CONSTANTS(flag2, int, 8)
    PUSH_CONSTANTS(flag3, int, 12)
END_PUSH_CONSTANTS()

struct InstanceData
{
    float4 instance_data;
};

struct DrawElementData
{
    float4 color;
    float2 scale;
    float2 translate;
    float2 uv_offset;
    float2 uv_size;
};

STRUCTURE_BUFFER(InstanceData, InstanceDataBuffer, ENTITY, 0);
STRUCTURE_BUFFER(DrawElementData, ElementDataBuffer, ENTITY, 1);

int GetElementID(int instance_id)
{
    return (int)(InstanceDataBuffer[instance_id].instance_data.x + 0.5);
}

float4 GetElementColor(int instance_id)
{
    return ElementDataBuffer[GetElementID(instance_id)].color;
}

float2 GetElementScale(int instance_id)
{
    return ElementDataBuffer[GetElementID(instance_id)].scale;
}

float2 GetElementTranslate(int instance_id)
{
    return ElementDataBuffer[GetElementID(instance_id)].translate;
}

float2 GetUVOffset(int instance_id)
{
    return ElementDataBuffer[GetElementID(instance_id)].uv_offset;
}

float2 GetUVSize(int instance_id)
{
    return ElementDataBuffer[GetElementID(instance_id)].uv_size;
}

VS_OUTPUT BoxVS(VS_INPUT input)
{
    VS_OUTPUT output;
    float2 new_position = input.in_pos * GetElementScale(input.instance_id) + GetElementTranslate(input.instance_id);

    output.sv_Position = float4(new_position * 2 - 1, 0, 1);
    output.sv_Position.y = -output.sv_Position.y;
    output.color = GetElementColor(input.instance_id);
    output.uv0 = GetUVOffset(input.instance_id) + input.in_pos * GetUVSize(input.instance_id);
    return output;
}

float4 BoxPS(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}

TEXTURE2D(image, PS);
float4 BoxPSWithImage(VS_OUTPUT input) : SV_TARGET
{
    SamplerState defaultSampler;
    return image.Sample(defaultSampler, input.uv0);
}

VS_OUTPUT TextVS(VS_INPUT input)
{
    VS_OUTPUT output;
    float2 new_position = input.in_pos * GetElementScale(input.instance_id) + GetElementTranslate(input.instance_id);

    output.sv_Position = float4(new_position * 2 - 1, 0, 1);
    output.sv_Position.y = -output.sv_Position.y;
    output.color = GetElementColor(input.instance_id);
    output.uv0 = GetUVOffset(input.instance_id) + input.in_pos * GetUVSize(input.instance_id);
    return output;
}

float4 TextPS(VS_OUTPUT input) : SV_TARGET
{
    SamplerState defaultSampler;
    return image.Sample(defaultSampler, input.uv0).r * input.color;
}