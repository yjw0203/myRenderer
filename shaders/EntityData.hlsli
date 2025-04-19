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
};

STRUCTURE_BUFFER(EntityData, EntityDataBuffer, ENTITY, 0);

float4x4 GetModelMatrix()
{
    return EntityDataBuffer[PUSH_CONSTANTS.entity_data_id].model_mat;
}
