#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"
#include "Engine/Engine/Public/Render/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

namespace yjw
{
    class PrimitiveComponent : public SceneRootComponent
    {
    public:
        virtual Primitive* GetPrimitive() { return nullptr; };
        virtual MaterialInstance* GetMaterialInstance() { return nullptr; };
    };
}
