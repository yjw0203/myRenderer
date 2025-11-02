#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/System/System.h"

class Meta() AnimationSystem : public System
{
    GENERATED_BODY()
public:
    virtual void Update(float deltaTime) override;
};
