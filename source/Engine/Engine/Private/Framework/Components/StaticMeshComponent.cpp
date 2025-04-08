#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"

namespace yjw
{
    void StaticMeshComponent::SetPrimitive(const char* url)
    {
        m_mesh_url = url;
    }

    const char* StaticMeshComponent::GetPrimitive()
    {
        return m_mesh_url.c_str();
    }
}