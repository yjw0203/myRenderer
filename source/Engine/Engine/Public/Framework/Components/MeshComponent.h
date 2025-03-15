#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"
#include "Engine/Engine/Public/Render/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
namespace yjw
{
    class MeshComponent : public IComponent
    {
    public:
        void SetMesh(Mesh* mesh);
        Mesh* GetMesh();
        
        Primitive* GetPrimitive();
        MaterialInstance* GetMaterialInstance();

    private:
        Mesh* m_mesh = nullptr;
        MaterialInstance* m_override_material = nullptr;
    };
}
