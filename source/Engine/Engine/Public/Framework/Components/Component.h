#pragma once
#include <vector>
#include "Engine/Math/Public/Math.h"
#include "Engine/Utils/Public/Object/Object.h"

namespace yjw
{
    class Meta() Component : public MObject
    {
        GENERATED_BODY();
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
