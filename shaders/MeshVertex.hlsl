#include "Global.hlsli"

struct MeshVertexInput  
{  
    float3 positon : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    int4 blend_indices : BLEND_INDICES;
    float4 blend_weights : BLEND_WEIGHTS;
    int blend_type : BLEND_TYPE;
};  
  
struct MeshVertexOutput  
{  
    float4 sv_Position : SV_POSITION;  
    float3 position : POSITION;  
    float3 normal : NORMAL;  
    float2 uv0 : TEXCOORD0;  
};  
  
Buffer<float4> boneMatrices : register(t3);

float4x4 GetBoneMatrix(int index)
{
    return float4x4(boneMatrices[index * 4], boneMatrices[index * 4 + 1], boneMatrices[index * 4 + 2], boneMatrices[index * 4 + 3]);
}

float4x4 GetBlendBoneMatrix(int4 indices, float4 weights, int blend_type)
{
    float4x4 bone_blend_matrix = 0;

    if (blend_type == 0)
    {
        bone_blend_matrix = GetBoneMatrix(indices.x);
    }
    else if (blend_type == 1)
    {
        bone_blend_matrix = weights.x * GetBoneMatrix(indices.x) + weights.y * GetBoneMatrix(indices.y);
    }
    else if (blend_type == 2)
    {
        bone_blend_matrix = weights.x * GetBoneMatrix(indices.x) + weights.y * GetBoneMatrix(indices.y) + weights.z * GetBoneMatrix(indices.z) + weights.w * GetBoneMatrix(indices.w);
    }
    else
    {
        float4x4 E =
        {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
        };
        bone_blend_matrix = E;
    }
    return bone_blend_matrix;

}

MeshVertexOutput VSMain(MeshVertexInput input)  
{  
    MeshVertexOutput output;  

    float4x4 bone_blend_matrix = 0;
    int4 indices = input.blend_indices;
    float4 weights = input.blend_weights;
    
    float4x4 bone_matrix = GetBlendBoneMatrix(input.blend_indices, input.blend_weights, input.blend_type);
    float4 model_position = mul(float4(input.positon, 1.0), bone_matrix);
    float3x3 bone_matrix_3x3 = float3x3(bone_matrix[0].xyz, bone_matrix[1].xyz, bone_matrix[2].xyz);
    
    output.sv_Position = mul(mul(model_position, viewMat) ,projectMat);
  
    output.position = model_position.xyz;
    output.normal = mul(input.normal, bone_matrix_3x3);
    output.uv0 = input.uv0;  
  
    return output;  
}

struct SimpleMeshVertexInput
{
    float3 positon : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
};

MeshVertexOutput SimpleVS(SimpleMeshVertexInput input)
{
    MeshVertexOutput output;
    
    float4 model_position = float4(input.positon, 1.0);
    
    output.sv_Position = mul(mul(model_position, viewMat), projectMat);
  
    output.position = model_position.xyz;
    output.normal = input.normal;
    output.uv0 = input.uv0;
  
    return output;
}