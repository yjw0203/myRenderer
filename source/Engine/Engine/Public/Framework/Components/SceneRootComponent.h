#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

namespace yjw
{
    class Meta() SceneRootComponent : public Component
    {
        GENERATED_BODY();
    public:
        const Transform& GetRelativeToParent();
    private:
        Transform m_relative{};
    };
}