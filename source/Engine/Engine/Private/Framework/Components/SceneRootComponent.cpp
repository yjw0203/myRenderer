#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"

void SceneRootComponent::SetRelativeTransform(const Transform& transform)
{
    m_relative = transform;
}

const Transform& SceneRootComponent::GetRelativeToParent()
{
    return m_relative;
}

const Transform& SceneRootComponent::GetTransform()
{
    if (m_parent)
    {
        return m_parent->GetTransform() /** m_relative*/;
    }
    return m_relative;
}
