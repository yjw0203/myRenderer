#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Render/Private/Mesh.h"
#include "Engine/Render/Private/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

namespace yjw
{
    class PrimitiveComponent : public SceneRootComponent
    {
    public:
        virtual const char* GetPrimitive() { return ""; }
    };
}
