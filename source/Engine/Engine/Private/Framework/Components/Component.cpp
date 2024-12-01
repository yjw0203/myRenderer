#include "Engine/Engine/Public/Framework/Components/Component.h"

namespace yjw
{
    Component* Component::GetParent()
    {
        return m_parent_component;
    }

    void Component::AddSubComponent(Component* component)
    {
        m_sub_components.push_back(component);
    }

    std::vector<Component*>& Component::GetSubComponents()
    {
        return m_sub_components;
    }

}