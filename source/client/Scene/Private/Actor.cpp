#include "Scene/Actor.h"

namespace yjw
{
    Actor::Actor()
    {
        m_component = new ActorComponent();
    }
    Actor::~Actor()
    {
        delete m_component;
    }
}