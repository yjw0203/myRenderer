#pragma once
#include <vector>
#include <string>
#include "Engine/Math/Public/Math.h"

namespace yjw
{
    class Bone
    {
    public:
        using List = std::vector<Bone>;

        std::string m_name;
        int m_index;
        Bone* m_parent;
        Transform   m_anim_pose;
        Transform   m_pose;
        Transform   m_init_pose;
        Matrix4x4   m_inverse_init;
        Matrix4x4   m_local;
    };
}