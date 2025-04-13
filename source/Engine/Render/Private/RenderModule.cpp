#include "Engine/Render/Public/RenderModule.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/Render/Private/MaterialManager.h"
#include "Engine/Render/Private/PrimitiveManager.h"
#include "Engine/Render/Private/View.h"
#include "Engine/Render/Private/Renderer/ForwardRenderer.h"
#include "Engine/Render/Private/Pass/PickPass.h"

namespace yjw
{
    ForwardRenderer* renderer = new ForwardRenderer();

    RPIContext g_context;
    void RenderModule::Startup()
    {
        m_render_system = new RenderSystem();
        m_material_manager = new MaterialManager();
        m_primitive_manager = new PrimitiveManager();

        m_render_system->initialize();
        renderer->Initialize();

        m_pick_pass = new PickPass();
        m_pick_pass->Initialize();

        g_context = RPICreateContext();
    }

    void RenderModule::Shutdown()
    {
        m_render_system->shutdown();
        delete m_render_system;
        delete m_material_manager;
        delete m_primitive_manager;
        delete m_pick_pass;
    }

    void RenderModule::Tick(float deltaTime)
    {
        m_render_system->tick(deltaTime);
        m_pick_pass->Submit();
        renderer->Render();
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

    ViewProxy* RenderModule::CreateView(void* window)
    {
        return new RViewProxy(window);
    }

    void RenderModule::Destroy(ViewProxy* view)
    {
        delete view;
    }

    void RenderModule::AttachView(ViewProxy* view)
    {
        DettachView(view);
        m_views.push_back((RViewProxy*)view);
        renderer->SetView(((RViewProxy*)view)->GetView());
        m_pick_pass->AttachView(((RViewProxy*)view)->GetView());
    }

    void RenderModule::DettachView(ViewProxy* view)
    {
        m_views.erase(std::remove_if(m_views.begin(), m_views.end(), 
            [&view](const RViewProxy* p) {return p == (RViewProxy*)view; }), 
            m_views.end());
    }

    void RenderModule::AddPendingHitRequest(const char* group_name, const RenderHitRequest& request)
    {
        m_pick_pass->AddPendingHitRequest(group_name, request);
    }

    void RenderModule::GetProcessedHitRequest(const char* group_name, std::vector<RenderHitRequest>& proccessed_request)
    {
        m_pick_pass->GetProcessedHitRequest(group_name, proccessed_request);
    }

    template<>
    IRenderModule* GetModule()
    {
        static RenderModule instance;
        return &instance;
    }

}