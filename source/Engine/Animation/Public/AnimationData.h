#pragma once
#include <vector>
#include <string>
#include "Engine/Math/Public/Math.h"

namespace yjw
{
    class BoneData
    {
    public:
        std::string name;
        int         index;
        Transform   binding_pose;
        Matrix4x4   inverse_init;
        int         parent_index;
    };

    class SkeletonData
    {
    public:
        std::vector<BoneData> bones;
    };

    class AnimationChannelData
    {
    public:
        std::string             name;
        std::vector<Vector3>    position_keys;
        std::vector<Quaternion> rotation_keys;
        std::vector<Vector3>    scaling_keys;
    };

    class AnimationClipData
    {
    public:
        int                               total_frame{ 0 };
        int                               node_count{ 0 };
        std::vector<AnimationChannelData> node_channels;
    };
}