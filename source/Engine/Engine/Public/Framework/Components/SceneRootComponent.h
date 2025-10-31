#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

namespace yjw
{
    class Meta() SceneRootComponent : public Component
    {
        GENERATED_BODY();
    public:
        void SetRelativeTransform(const Transform& transform);
        const Transform& GetRelativeToParent();
        const Transform& GetTransform();
    private:
        Meta()
        Transform m_relative{};

        SceneRootComponent* m_parent = nullptr;
    };
}