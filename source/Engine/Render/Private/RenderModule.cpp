#include "Engine/Render/Public/RenderModule.h"
#include "Engine/Render/Public/yjw_render_system.h"

namespace yjw
{
    void RenderModule::Startup()
    {
        m_render_system = new RenderSystem();
    }

    void RenderModule::Shutdown()
    {
        m_render_system->shutdown();
        delete m_render_system;
    }

    void RenderModule::AttachScene(Scene* scene)
    {
        m_render_system->AttachScene(scene);
        m_render_system->initialize();
    }

    void RenderModule::Tick(float deltaTime)
    {
        m_render_system->tick(deltaTime);
    }

    template<>
    IRenderModule* CreateModule()
    {
        return new RenderModule();
    }

}