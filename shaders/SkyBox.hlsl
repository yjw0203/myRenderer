#include "Global.hlsli"

struct VS_INPUT
{
    float3 in_pos : POSITION;
};
  
struct VS_OUTPUT
{
    float4 sv_Position : SV_POSITION;
    float3 tex_coord : TEXCOORD0;
};

VS_OUTPUT VSMain(VS_INPUT input)  
{  
    VS_OUTPUT output{};
    output.tex_coord = input.in_pos;
    float4 pos = mul(mul(float4(input.in_pos,1.0f),viewMatWithoutTranslation),projectMat);
    pos.z = pos.w * 0.99999;
    output.sv_Position = pos;
    return output;  
}

TEXTURECUBE(skybox, PS);

float4 PSMain(VS_OUTPUT ps_in) : SV_Target
{
    SamplerState defaultSampler;
    return skybox.Sample(defaultSampler, ps_in.tex_coord);
}