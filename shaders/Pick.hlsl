#include "Global.hlsli"

BEGIN_PUSH_CONSTANTS()
PUSH_CONSTANTS(flag, int, 0)
END_PUSH_CONSTANTS()

struct MeshVertexOutput
{
    float4 sv_Position : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : UV0;
};

int PSMain(MeshVertexOutput ps_in) : SV_TARGET
{
    return PUSH_CONSTANTS.flag;
}