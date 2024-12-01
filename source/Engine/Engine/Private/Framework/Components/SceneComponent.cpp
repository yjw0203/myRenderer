#include "Engine/Engine/Public/Framework/Components/SceneComponent.h"

namespace yjw
{
    const Transform& SceneComponent::GetRelativeToParent()
    {
        return m_relative;
    }
}