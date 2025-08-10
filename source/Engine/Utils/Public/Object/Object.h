#pragma once
#include "Engine/Utils/Public/Meta/Meta.h"

namespace yjw
{
    class MObject
    {
    public:
        virtual void Serialize(class Archive& Ar);
    };
}
