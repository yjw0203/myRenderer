#include "Global.hlsli"

struct MeshVertexOutput  
{  
    float4 sv_Position : SV_POSITION;  
    float3 position : POSITION;  
    float3 normal : NORMAL;  
    float2 uv0 : TEXCOORD0;  
};  
  
cbuffer material : register(b3)  
{  
    float2 metallic_roughness;
};  
  
Texture2D albedoTex : register(t0,space1);

struct PS_OUTPUT  
{
    float4 color : SV_TARGET0;
};

struct GBufferData
{
    float3 world_positon;
    float3 albedo;
    float3 normal;
    float metallic;
    float roughness;
};

float D_GGX_TR(float3 N, float3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = 3.14159265359f * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float NdotV, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);
}

float3 calculateLighting(float3 lightDir, float3 viewDir, float3 normal, float3 albedo, float metallic, float roughness)
{
    // Diffuse component
    float3 diffuseColor = albedo * (1.0f - metallic);
    
    // Lighting calculations
    float3 halfwayDir = normalize(lightDir + viewDir);
    float NdotL = max(dot(normal, lightDir), 0.0f);
    float NdotV = max(dot(normal, viewDir), 0.0f);
    float NdotH = max(dot(normal, halfwayDir), 0.0f);
    float VdotH = max(dot(viewDir, halfwayDir), 0.0f);
    
    // Microfacet Distribution (GGX)
    float NDF = D_GGX_TR(normal,halfwayDir,roughness);
    
    // Geometric Shadowing (Smith)
    float k = (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;
    float G = GeometrySmith(normal,viewDir,lightDir,k);
    
    // Fresnel Reflection (Schlick)
    float3 F0 = lerp(float3(0.04f,0.04f,0.04f), albedo, metallic);
    float3 F = fresnelSchlick(NdotV,F0);
    
    // Final Lighting Calculation
    float3 diffuse = NdotL == 0.0f ? 0.0f : (diffuseColor / 3.14159265359f) * NdotL;
    float3 specular = NdotL == 0.0f ? 0.0f : (F * G * NDF) / (4.0f * NdotL * NdotV);
    float3 environment = float3(0.1f, 0.1f, 0.1f) * albedo;
    
    return (diffuse + specular) * NdotL * lightColor + environment;
}

float3 calculateShading(GBufferData gbuffer_data)
{
    float3 worldSpacePos = gbuffer_data.world_positon;
    float3 lightDir = normalize(lightDirection);
    float3 viewDir = normalize(cameraPos - worldSpacePos.xyz); 

    return calculateLighting(lightDir,viewDir,gbuffer_data.normal,gbuffer_data.albedo,gbuffer_data.metallic,gbuffer_data.roughness);
}
  
PS_OUTPUT PSMain(MeshVertexOutput input)  
{  
    SamplerState defaultSampler;
    GBufferData gbuffer_data;
    gbuffer_data.world_positon = input.position;
    gbuffer_data.albedo = albedoTex.Sample(defaultSampler, input.uv0).xyz;  
    gbuffer_data.metallic = metallic_roughness.x;
    gbuffer_data.roughness = metallic_roughness.y;
    gbuffer_data.normal = input.normal;
    
    PS_OUTPUT output;  
    output.color = float4(calculateShading(gbuffer_data),1);
    return output;  
}
