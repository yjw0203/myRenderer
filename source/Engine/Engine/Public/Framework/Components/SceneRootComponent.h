#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

namespace yjw
{
    class SceneRootComponent : public IComponent
    {
    public:
        const Transform& GetRelativeToParent();
    private:
        Transform m_relative{};
    };
}