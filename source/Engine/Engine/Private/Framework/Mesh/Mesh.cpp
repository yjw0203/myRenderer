#include "Engine/Engine/Public/Framework/Mesh/Mesh.h"

namespace yjw
{
    Mesh::Mesh(Primitive* primitive, MaterialInstance* material)
    {
        m_primitive = primitive;
        m_material_instance = material;
    }

    void Mesh::SetMaterialInstance(MaterialInstance* material_instance)
    {
        m_material_instance = material_instance;
    }

    MaterialInstance* Mesh::GetMaterialInstance()
    {
        return m_material_instance;
    }

    void Mesh::SetPrimitive(Primitive* primitive)
    {
        m_primitive = primitive;
    }

    Primitive* Mesh::GetPrimitive()
    {
        return m_primitive;
    }
}