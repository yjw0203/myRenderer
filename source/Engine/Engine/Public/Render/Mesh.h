#pragma once
#include "Engine/Engine/Public/Render/Material.h"
#include "Engine/Engine/Public/Render/Primitive.h"
#include "Engine/Utils/Public/Define.h"

namespace yjw
{
    class Mesh
    {
    public:
        Mesh(Primitive* primitive, MaterialInstance* material);
        void SetMaterialInstance(MaterialInstance* material_instance);
        MaterialInstance* GetMaterialInstance();
        void SetPrimitive(Primitive* primitive);
        Primitive* GetPrimitive();
    private:
        MaterialInstance* m_material_instance = nullptr;
        Primitive* m_primitive = nullptr;
    };
}