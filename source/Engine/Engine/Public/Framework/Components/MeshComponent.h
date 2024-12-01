#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneComponent.h"
#include "Engine/Engine/Public/Framework/Mesh/Mesh.h"
namespace yjw
{
    class MeshComponent : public SceneComponent
    {
    public:
        void SetMesh(Mesh* mesh);
        
        Primitive* GetPrimitive();
        MaterialInstance* GetMaterialInstance();

    private:
        Mesh* m_mesh = nullptr;
        MaterialInstance* m_override_material = nullptr;
    };
}
