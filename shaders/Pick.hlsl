#include "Global.hlsli"

BEGIN_PUSH_CONSTANTS()
PUSH_CONSTANTS(flag0, int, 0)
PUSH_CONSTANTS(flag1, int, 4)
PUSH_CONSTANTS(flag2, int, 8)
PUSH_CONSTANTS(flag3, int, 12)
END_PUSH_CONSTANTS()

struct MeshVertexOutput
{
    float4 sv_Position : SV_POSITION;
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : UV0;
};

int4 PSMain(MeshVertexOutput ps_in) : SV_TARGET
{
    return int4(PUSH_CONSTANTS.flag0,PUSH_CONSTANTS.flag1,PUSH_CONSTANTS.flag2,PUSH_CONSTANTS.flag3);
}