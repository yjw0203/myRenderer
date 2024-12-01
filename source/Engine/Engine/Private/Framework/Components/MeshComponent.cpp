#include "Engine/Engine/Public/Framework/Components/MeshComponent.h"

namespace yjw
{
    void MeshComponent::SetMesh(Mesh* mesh)
    {
        m_mesh = mesh;
    }

    Primitive* MeshComponent::GetPrimitive()
    {
        return m_mesh->GetPrimitive();
    }

    MaterialInstance* MeshComponent::GetMaterialInstance()
    {
        if (m_override_material)
        {
            return m_override_material;
        }
        return m_mesh->GetMaterialInstance();
    }
}