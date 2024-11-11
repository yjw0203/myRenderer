#include "Engine/Framework/Components/SceneComponent.h"

namespace yjw
{
    const Transform& SceneComponent::GetRelativeToParent()
    {
        return m_relative;
    }
}