#pragma once
#include <vector>
#include "Engine/Math/Public/Math.h"
#include "Engine/Utils/Public/Object/Object.h"
namespace yjw
{
    class Component : public MObject
    {
    public:
        virtual ~Component() {};

        template<typename T>
        T* CastTo()
        {
            // to optimize using type reflection
            return dynamic_cast<T*>(this);
        }

    private:
    };
}
