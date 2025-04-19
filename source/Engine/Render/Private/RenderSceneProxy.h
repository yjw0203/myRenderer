#pragma once
#include "Engine/Render/Private/Scene.h"
#include <functional>

namespace yjw
{
    class IRenderer;
    class RenderSceneProxy
    {
    public:
        RenderSceneProxy() {}
        RenderSceneProxy(RdScene* scene) : m_scene(scene) {}

        template<typename T,typename Func>
        void SubmitOpaque(T* This ,Func func)
        {
            std::vector<DrawItem> draw_items;
            m_scene->GetDrawItems(draw_items);
            for (DrawItem& item : draw_items)
            {
                auto bfunc = std::bind(func, This, &item);
                bfunc();
            }
        }
    private:
        RdScene* m_scene{};
    };
}