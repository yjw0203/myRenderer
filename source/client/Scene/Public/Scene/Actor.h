#pragma once
#include <vector>
#include "Component.h"

namespace yjw
{
    class ActorComponent : Component
    {

    };

    class Actor
    {
    public:
        Actor();
        ~Actor();
    private:
        Transform m_transform;
        ActorComponent* m_component;
    };
}
