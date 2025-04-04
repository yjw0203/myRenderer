struct VS_INPUT
{
    float2 in_pos : POSITION;
};
  
struct VS_OUTPUT
{
    float4 sv_Position : SV_POSITION;
    float2 pos : UV0;
};
  
VS_OUTPUT VSMain(VS_INPUT input)
{
    VS_OUTPUT output;
    output.sv_Position = float4(input.in_pos, 1.0f, 1.0f);
    output.pos = input.in_pos;
    return output;
}

Texture2D color_map;

float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

float4 PSMain(VS_OUTPUT input) : SV_Target
{
    SamplerState defaultSampler;
    float2 uv = (input.pos.xy + float2(1.0, 1.0)) / 2;
    return float4(ACESFilm(color_map.Sample(defaultSampler, uv).xyz), 1.f);
}