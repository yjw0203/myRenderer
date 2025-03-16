#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"

namespace yjw
{
    void StaticMeshComponent::SetMesh(Mesh* mesh)
    {
        m_mesh = mesh;
    }

    Mesh* StaticMeshComponent::GetMesh()
    {
        return m_mesh;
    }

    Primitive* StaticMeshComponent::GetPrimitive()
    {
        return m_mesh->GetPrimitive();
    }

    MaterialInstance* StaticMeshComponent::GetMaterialInstance()
    {
        if (m_override_material)
        {
            return m_override_material;
        }
        return m_mesh->GetMaterialInstance();
    }
}