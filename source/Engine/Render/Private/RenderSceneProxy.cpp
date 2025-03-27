#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/Render/Private/Renderer/IRenderer.h"

namespace yjw
{
    void RenderSceneProxy::SubmitOpaque(IRenderer* renderer)
    {
        for (RenderEntity* entity : m_scene->GetMeshes())
        {
            entity->Build();
            if (entity->ReadyRender())
            {
                renderer->Submit(entity);
            }
        }
    }
}