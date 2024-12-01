#pragma once
#include <vector>
#include "Engine/Engine/Public/Framework/Components/SceneComponent.h"
#include <string>
namespace yjw
{
    class Actor
    {
    public:
        Actor(char* name);
        ~Actor();
        const char* GetName() const;
        SceneComponent* GetRootComponent();
    private:
        SceneComponent* m_root_component = nullptr;

        std::string m_name;
    };
}
