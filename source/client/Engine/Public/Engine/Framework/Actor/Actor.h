#pragma once
#include <vector>
#include "Engine/Framework/Components/SceneComponent.h"

namespace yjw
{
    class Actor
    {
    public:
        Actor();
        ~Actor();
        SceneComponent* GetRootComponent();
    private:
        SceneComponent* m_root_component = nullptr;
    };
}
