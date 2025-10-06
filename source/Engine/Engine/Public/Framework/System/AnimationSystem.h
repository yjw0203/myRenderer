#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/System/System.h"

namespace yjw
{
    class AnimationSystem : public System
    {
    public:
        virtual void Update(float deltaTime) override;
    };

}