#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"

namespace yjw
{
    const Transform& SceneRootComponent::GetRelativeToParent()
    {
        return m_relative;
    }
}