#include "Scene/Component.h"

namespace yjw
{
    Component* Component::GetParent()
    {
        return m_parent_component;
    }

}