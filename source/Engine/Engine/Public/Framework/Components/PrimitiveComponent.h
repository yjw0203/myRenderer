#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Render/Private/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"

class Meta() PrimitiveComponent : public SceneRootComponent
{
    GENERATED_BODY();
public:
    virtual const char* GetPrimitive() { return ""; }
};

