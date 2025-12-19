#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

class Meta() SceneRootComponent : public Component
{
    GENERATED_BODY();
public:
    void SetRelativeTransform(const Transform& transform);
    const Transform& GetRelativeToParent();
    const Transform& GetTransform();
private:
    Property()
    Transform m_relative{};

    SceneRootComponent* m_parent = nullptr;
};
