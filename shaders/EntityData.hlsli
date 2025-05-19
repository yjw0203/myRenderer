#include "Support.hlsli"

BEGIN_PUSH_CONSTANTS()
    PUSH_CONSTANTS(entity_data_id, int, 0)
    PUSH_CONSTANTS(flag1, int, 4)
    PUSH_CONSTANTS(flag2, int, 8)
    PUSH_CONSTANTS(pick_flag, int, 12)
END_PUSH_CONSTANTS()

struct EntityData
{
    float4x4 model_mat;
    int skeletal_start_id;
};

STRUCTURE_BUFFER(EntityData, EntityDataBuffer, ENTITY, 0);
STRUCTURE_BUFFER(float4x4, SkeletalMatrixBuffer, ENTITY, 1);

float4x4 GetModelMatrix()
{
    return EntityDataBuffer[PUSH_CONSTANTS.entity_data_id].model_mat;
}

float4x4 GetSkeletonMatrix(int index)
{
    if (EntityDataBuffer[PUSH_CONSTANTS.entity_data_id].skeletal_start_id == 0)
    {
        return SkeletalMatrixBuffer[0];
    }
    return SkeletalMatrixBuffer[EntityDataBuffer[PUSH_CONSTANTS.entity_data_id].skeletal_start_id + index];

}