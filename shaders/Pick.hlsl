#include "Global.hlsli"
#include "EntityData.hlsli"

struct MeshVertexOutput
{
    float4 sv_Position : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : UV0;
};

int4 PSMain(MeshVertexOutput ps_in) : SV_TARGET
{
    return int4(PUSH_CONSTANTS.pick_flag,PUSH_CONSTANTS.pick_flag,PUSH_CONSTANTS.pick_flag,PUSH_CONSTANTS.pick_flag);
}