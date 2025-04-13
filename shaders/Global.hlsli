#include "Support.hlsli"

CBUFFER(camera,COMMON, 0)
{
    float4x4 viewMat;
    float4x4 viewMatWithoutTranslation;
    float4x4 projectMat;
    float3 cameraPos;
    float2 near_far;
};

CBUFFER(light,COMMON, 1)
{
    float3 lightDirection;
    float3 lightColor;
};

CBUFFER(option,COMMON, 2)
{
    float2 screenSize;  
};