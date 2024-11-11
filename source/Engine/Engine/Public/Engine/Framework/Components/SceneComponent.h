#pragma once
#include "Engine/Framework/Components/Component.h"

namespace yjw
{
    class SceneComponent : public Component
    {
    public:
        const Transform& GetRelativeToParent();
    private:
        Transform m_relative{};
    };
}