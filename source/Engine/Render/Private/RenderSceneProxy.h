#pragma once
#include "Engine/Engine/Public/Render/Scene.h"

namespace yjw
{
    class IRenderer;
    class RenderSceneProxy
    {
    public:
        RenderSceneProxy() {}
        RenderSceneProxy(Scene* scene) : m_scene(scene) {}
        void SubmitOpaque(IRenderer* renderer);
    private:
        Scene* m_scene{};
    };
}