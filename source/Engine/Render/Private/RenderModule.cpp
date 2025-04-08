#include "Engine/Render/Public/RenderModule.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/Render/Private/MaterialManager.h"
#include "Engine/Render/Private/PrimitiveManager.h"

namespace yjw
{
    void RenderModule::Startup()
    {
        m_render_system = new RenderSystem();
        m_material_manager = new MaterialManager();
        m_primitive_manager = new PrimitiveManager();
    }

    void RenderModule::Shutdown()
    {
        m_render_system->shutdown();
        delete m_render_system;
        delete m_material_manager;
        delete m_primitive_manager;
    }

    void RenderModule::AttachScene(SceneInterface* scene)
    {
        m_render_system->AttachScene((Scene*)scene);
        m_render_system->initialize();
    }

    void RenderModule::AttachUI(rhi::ImGuiUI* ui)
    {
        m_render_system->AttachUI(ui);
    }

    void RenderModule::Tick(float deltaTime)
    {
        m_render_system->tick(deltaTime);
    }

    SceneInterface* RenderModule::CreateScene()
    {
        return new Scene(this);
    }

    MaterialHandle RenderModule::LoadMaterial(const char* url)
    {
        return m_material_manager->LoadMaterial(url);
    }

    void RenderModule::UnloadMaterial(MaterialHandle handle)
    {
        m_material_manager->UnloadMaterial(handle);
    }

    MeshHandle RenderModule::LoadMesh(const char* url)
    {
        return m_primitive_manager->LoadMesh(url);
    }

    void RenderModule::UnloadMesh(MeshHandle handle)
    {
        m_primitive_manager->UnloadMesh(handle);
    }

    template<>
    IRenderModule* GetModule()
    {
        static RenderModule instance;
        return &instance;
    }

}