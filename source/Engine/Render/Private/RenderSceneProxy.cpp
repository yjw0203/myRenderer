#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/Render/Private/Renderer/IRenderer.h"

namespace yjw
{
    void RenderSceneProxy::SubmitOpaque(IRenderer* renderer)
    {
        for (Mesh* mesh : m_scene->GetMeshes())
        {
            RenderEntity entity{};
            entity.m_material = mesh->GetMaterialInstance();
            entity.m_resource_binding = mesh->GetMaterialInstance()->GetResourceBinding();
            entity.m_primitive_binding = mesh->GetPrimitive()->GetPrimitiveBinding();
            if (entity.m_material && entity.m_resource_binding && entity.m_primitive_binding)
            {
                renderer->Submit(&entity);
            }
        }
    }
}