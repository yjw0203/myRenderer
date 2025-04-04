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
  
Texture2D albedo_map;  
Texture2D normal_map;  
Texture2D diffuse_map;  
Texture2D specular_map;  
Texture2D ambient_map;  
Texture2D depth_map;
  
cbuffer camera
{  
    float4x4 viewMat;  
    float4x4 projectMat;  
    float3 cameraPos;  
};  
  
cbuffer light
{  
    float3 lightPos;  
    float3 lightColor;  
};  
  
cbuffer option
{  
    float2 screenSize;  
};  
  
float4 PSMain(VS_OUTPUT input) : SV_Target  
{  
    SamplerState defaultSampler;
    float2 uv = input.sv_Position.xy/screenSize;  
    float4 screenSpacePos = float4(uv * 2.0f - 1.0f, depth_map.Sample(defaultSampler, uv).r, 1.0f);  
    float4x4 screenToWorld = mul(transpose(projectMat), transpose(viewMat));  
    float4 worldSpacePos = mul(screenToWorld, screenSpacePos);  
    worldSpacePos /= worldSpacePos.w;  
  
    float3 ambientStrength = ambient_map.Sample(defaultSampler, uv).xyz;  
    float3 ambient = ambientStrength * lightColor;  
  
    float3 norm = normalize(normal_map.Sample(defaultSampler, uv).xyz * 2.0f - 1.0f);
    float3 lightDir = normalize(lightPos - worldSpacePos.xyz);  
    float diff = max(dot(norm, lightDir), 0.0f);  
    float3 diffuse = diff * lightColor * diffuse_map.Sample(defaultSampler, uv).xyz;  
  
    float4 specularStrength = specular_map.Sample(defaultSampler, uv);  
    float3 viewDir = normalize(cameraPos - worldSpacePos.xyz);  
    float3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), specularStrength.w);  
    float3 specular = specularStrength.xyz * spec * lightColor;  
  
    float4 objectColor = albedo_map.Sample(defaultSampler, uv);  
    float3 result = (ambient + diffuse + specular) * objectColor.rgb;  
    return float4(result, 1.0f);  
}