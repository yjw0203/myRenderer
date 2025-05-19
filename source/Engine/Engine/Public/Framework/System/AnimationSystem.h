#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

namespace yjw
{
    class AnimationSystem : public SystemBase
    {
    public:
        virtual void Update(float deltaTime) override;
    };

}