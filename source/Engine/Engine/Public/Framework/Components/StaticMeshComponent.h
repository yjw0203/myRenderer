#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Render/Private/Mesh.h"
#include "Engine/Render/Private/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
namespace yjw
{
    class StaticMeshComponent : public PrimitiveComponent
    {
    public:
        void SetPrimitive(const char* url);
        
        virtual const char* GetPrimitive() override;

    private:
        std::string m_mesh_url{};
    };
}
