#include "Engine/Framework/Actor/Actor.h"

namespace yjw
{
    Actor::Actor()
    {
        m_root_component = new SceneComponent();
    }
    Actor::~Actor()
    {
        delete m_root_component;
    }

    SceneComponent* Actor::GetRootComponent()
    {
        return m_root_component;
    }
}