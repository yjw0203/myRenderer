#pragma once
#include <vector>
#include "Math/Math.h"
namespace yjw
{
    class Component
    {
    public:
        Component* GetParent();
    private:
        Transform m_transform;
        Component* m_parent_component{};
        std::vector<Component*> m_sub_components;
    };
}
