cbuffer camera
{
    float4x4 viewMat;
    float4x4 viewMatWithoutTranslation;
    float4x4 projectMat;
    float3 cameraPos;
};  

cbuffer light
{
    float3 lightDirection;
    float3 lightColor;
};
