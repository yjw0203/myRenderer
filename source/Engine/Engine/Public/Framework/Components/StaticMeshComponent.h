#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"
#include "Engine/Engine/Public/Render/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
namespace yjw
{
    class StaticMeshComponent : public PrimitiveComponent
    {
    public:
        void SetMesh(Mesh* mesh);
        Mesh* GetMesh();
        
        virtual Primitive* GetPrimitive() override;
        virtual MaterialInstance* GetMaterialInstance() override;

    private:
        Mesh* m_mesh = nullptr;
        MaterialInstance* m_override_material = nullptr;
    };
}
