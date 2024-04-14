cbuffer camera : register(b0)  
{  
    float4x4 viewMat;  
    float4x4 projectMat;  
    float3 cameraPos;  
};  
  
struct VS_INPUT  
{  
    float3 in_pos : POSITION;  
    float3 in_normal : NORMAL;  
    float2 in_uv : TEXCOORD0;  
};  
  
struct VS_OUTPUT  
{  
    float4 sv_Position : SV_POSITION;  
    float3 pos : POSITION;  
    float3 normal : NORMAL;  
    float2 uv : TEXCOORD0;  
};  
  
VS_OUTPUT VSMain(VS_INPUT input)  
{  
    VS_OUTPUT output;  
  
    output.sv_Position = mul(mul(float4(input.in_pos, 1.0), viewMat), projectMat);  
  
    output.pos = input.in_pos;  
    output.normal = input.in_normal;  
    output.uv = input.in_uv;  
  
    return output;  
}
  
cbuffer material : register(b3)  
{  
    float4 material_diffuse;  
    float4 material_specular;  
    float3 material_ambient;  
};  
  
Texture2D albedoTex : register(t0);

struct PS_OUTPUT  
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 diffuse : SV_TARGET2;
    float4 specular : SV_TARGET3;
    float4 ambient : SV_TARGET4;
};  
  
PS_OUTPUT PSMain(VS_OUTPUT input)  
{  
    SamplerState defaultSampler;
    PS_OUTPUT output;  

    output.albedo = albedoTex.Sample(defaultSampler, input.uv);  
  
    output.normal = float4(input.normal, 1.0);  

    output.diffuse = material_diffuse;  
    output.specular = material_specular;  
    output.ambient = float4(material_ambient, 1.0);  
  
    return output;  
}
