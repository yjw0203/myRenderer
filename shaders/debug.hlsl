cbuffer camera
{
    float4x4 viewMat;
    float4x4 projectMat;
    float3 cameraPos;
};  

struct VertexOut
{
    float4 position : SV_POSITION;
};

VertexOut VSMain(float3 positon : POSITION)
{
    VertexOut output;
    output.position = mul(mul(float4(positon, 1.0), viewMat), projectMat);
    return output;
}

struct PixelOut
{
    float4 color : SV_Target0;
};
  
PixelOut PSMain(VertexOut input)
{
    PixelOut output;
    output.color = float4(1, 0, 0, 1);
    return output;
}