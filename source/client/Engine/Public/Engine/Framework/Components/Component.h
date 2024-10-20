#pragma once
#include <vector>
#include "Math/Math.h"
namespace yjw
{
    class Component
    {
    public:
        virtual ~Component() {};
        Component* GetParent();
        void AddSubComponent(Component* component);
        std::vector<Component*>& GetSubComponents();

        template<typename T>
        T* CastTo()
        {
            // to optimize using type reflection
            return dynamic_cast<T*>(this);
        }

    private:
        Component* m_parent_component{};
        std::vector<Component*> m_sub_components;
    };
}
