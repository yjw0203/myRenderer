#include "Engine/Engine/Public/Framework/Actor/Actor.h"

namespace yjw
{
    Actor::Actor(char* name)
        : m_name(name)
    {
        m_root_component = new SceneComponent();
    }

    Actor::~Actor()
    {
        delete m_root_component;
    }

    const char* Actor::GetName() const
    {
        return m_name.c_str();
    }

    SceneComponent* Actor::GetRootComponent()
    {
        return m_root_component;
    }
}