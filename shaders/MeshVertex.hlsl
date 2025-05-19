#include "Global.hlsli"
#include "EntityData.hlsli"

struct MeshVertexInput  
{  
    float3 positon : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv0 : UV0;
    uint blend_indices : BLEND_INDICES;
    uint blend_weights : BLEND_WEIGHTS;
    uint blend_indices1 : BLEND_INDICES1;
    uint blend_weights1 : BLEND_WEIGHTS1;
};  
  
struct MeshVertexOutput  
{  
    float4 sv_Position : SV_POSITION;  
    float3 position : POSITION;  
    float3 normal : NORMAL;  
    float2 uv0 : UV0;  
};

int4 UnpackBlendIndices(uint data)
{
    int4 indices;
    indices.x = int(data >> 24) & 0xFF;
    indices.y = int((data >> 16) & 0xFF);
    indices.z = int((data >> 8) & 0xFF);
    indices.w = int(data & 0xFF);
    return indices;
}

float4 UnpackBlendWeights(uint data)
{
    float4 weights;
    weights.x = float((data >> 24) & 0xFF) / 255.0f;
    weights.y = float((data >> 16) & 0xFF) / 255.0f;
    weights.z = float((data >> 8) & 0xFF) / 255.0f;
    weights.w = float(data & 0xFF) / 255.0f;
    return weights;
}

float4x4 GetBlendBoneMatrix(int4 indices, float4 weights)
{
    float4x4 bone_blend_matrix = 0;
    bone_blend_matrix = weights.x * GetSkeletonMatrix(indices.x) + weights.y * GetSkeletonMatrix(indices.y) + weights.z * GetSkeletonMatrix(indices.z) + weights.w * GetSkeletonMatrix(indices.w);
    return bone_blend_matrix;
}

MeshVertexOutput SkeletalVS(MeshVertexInput input)
{  
    MeshVertexOutput output;  

    float4x4 bone_blend_matrix = 0;
    int4 indices = input.blend_indices;
    float4 weights = input.blend_weights;
    float4x4 model_mat = GetModelMatrix();
    float3x3 model_mat_3x3 = float3x3(model_mat[0].xyz, model_mat[1].xyz, model_mat[2].xyz);
    
    float4x4 bone_matrix = GetBlendBoneMatrix(UnpackBlendIndices(input.blend_indices), UnpackBlendWeights(input.blend_weights)) + GetBlendBoneMatrix(UnpackBlendIndices(input.blend_indices1), UnpackBlendWeights(input.blend_weights1));
    float4 model_position = mul(float4(input.positon, 1.0), bone_matrix);
    float3x3 bone_matrix_3x3 = float3x3(bone_matrix[0].xyz, bone_matrix[1].xyz, bone_matrix[2].xyz);
    
    model_position = mul(model_position, model_mat);

    output.sv_Position = mul(mul(model_position, viewMat), projectMat);

    output.position = model_position.xyz;
    output.normal = mul(mul(input.normal, bone_matrix_3x3), model_mat_3x3);
    output.uv0 = input.uv0;  
  
    return output;  
}

struct SimpleMeshVertexInput
{
    float3 positon : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : UV0;
};

MeshVertexOutput SimpleVS(SimpleMeshVertexInput input)
{
    MeshVertexOutput output;
    float4x4 model_mat = GetModelMatrix();
    float3x3 model_mat_3x3 = float3x3(model_mat[0].xyz, model_mat[1].xyz, model_mat[2].xyz);

    float4 model_position = float4(input.positon, 1.0);
    
    model_position = mul(model_position, GetModelMatrix());

    output.sv_Position = mul(mul(model_position, viewMat), projectMat);
  
    output.position = model_position.xyz;
    output.normal = mul(input.normal, model_mat_3x3);
    output.uv0 = input.uv0;
  
    return output;
}