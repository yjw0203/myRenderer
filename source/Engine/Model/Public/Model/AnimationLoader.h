#pragma once
#include "Animation/Animation.h"

namespace yjw
{
    class VMD
    {
    public:
        void Load(std::string filePath, std::string fileName);
        std::shared_ptr<SkeletonAnimation> m_animation;
    };
}