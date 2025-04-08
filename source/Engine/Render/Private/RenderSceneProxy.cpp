#include "Engine/Render/Private/RenderSceneProxy.h"
#include "Engine/Render/Private/Renderer/IRenderer.h"

namespace yjw
{
    void RenderSceneProxy::SubmitOpaque(IRenderer* renderer)
    {
        std::vector<DrawItem> draw_items;
        m_scene->GetDrawItems(draw_items);
        for (DrawItem& item : draw_items)
        {
            renderer->Submit(&item);
        }
    }
}