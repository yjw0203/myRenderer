struct VS_INPUT  
{  
    float2 in_pos : POSITION;
};  
  
struct VS_OUTPUT  
{  
    float4 sv_Position : SV_POSITION;
    float2 pos : TEXCOORD0;
};  
  
VS_OUTPUT VSMain(VS_INPUT input)  
{  
    VS_OUTPUT output;
    output.sv_Position = float4(input.in_pos, 1.0f, 1.0f);  
    output.pos = input.in_pos;  
    return output;  
}

Texture2D applyTex;
  
float4 PSMain(VS_OUTPUT input) : SV_Target  
{  
    SamplerState defaultSampler;
    float2 uv = normalize(input.sv_Position.xy/input.sv_Position.w);
    return applyTex.Sample(defaultSampler,uv);
}