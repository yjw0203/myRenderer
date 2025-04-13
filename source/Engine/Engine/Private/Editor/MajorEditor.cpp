#include "Engine/Engine/Private/Editor/MajorEditor.h"
#include "Engine/Engine/Public/Framework/Render/IRenderModule.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Private/Editor/yjw_editor_ui.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"

#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Engine/Public/Framework/Render/IRenderModule.h"
#include "Engine/Engine/Public/Window.h"
namespace yjw
{
    void MajorEditor::Startup()
    {
        m_input_dispatcher.Register();
        m_input_dispatcher.SetOnClicked(std::bind(&MajorEditor::OnClicked, this, std::placeholders::_1, std::placeholders::_2));

        m_world = new World();
        m_ui = new EditorUI(m_world);
        m_window = new Window();

        m_view = GetModule<IRenderModule>()->CreateView(m_window->GetWindowHandle());
        m_view->AttachScene(m_world->GetScene());
        m_view->AttachUI(m_ui);

        GetModule<IRenderModule>()->AttachView(m_view);

        m_world->GetLevel()->SpawnActor<MeshActor>("naxita", "naxita/naxita.mesh.ast");
        m_world->GetLevel()->SpawnActor<MeshActor>("heita", "heita/heita.mesh.ast");
    }

    void MajorEditor::Destroy()
    {
        GetModule<IRenderModule>()->DettachView(m_view);
        delete m_world;
        delete m_ui;
        delete m_window;
    }

    void MajorEditor::Tick()
    {
        std::vector<RenderHitRequest> proccessed_request;
        GetModule<IRenderModule>()->GetProcessedHitRequest("select actor", proccessed_request);
        if (!proccessed_request.empty())
        {
            if (proccessed_request.back().m_result[0])
            {
                SelectActor(proccessed_request.back().m_result[0]);
            }
            else
            {
                UnselectActor();
            }
            printf("%d %d %d %d\n", proccessed_request.back().m_result[0], proccessed_request.back().m_result[1], proccessed_request.back().m_result[2], proccessed_request.back().m_result[3]);
        }
    }

    void MajorEditor::SelectActor(int actor_id)
    {
        if (m_select_actor_id)
        {
            UnselectActor();
        }
        Actor* actor = m_world->GetLevel()->GetActorById(actor_id);
        if (actor)
        {
            m_world->GetScene()->UpdateEntityRenderMask(actor->GetSceneEntity(), RenderMaskBits::highlight, true);
            m_select_actor_id = actor_id;
        }
    }

    void MajorEditor::UnselectActor()
    {
        Actor* actor = m_world->GetLevel()->GetActorById(m_select_actor_id);
        if (actor)
        {
            m_world->GetScene()->UpdateEntityRenderMask(actor->GetSceneEntity(), RenderMaskBits::highlight, false);
        }
    }

    void MajorEditor::OnClicked(float x, float y)
    {
        RenderHitRequest req{};
        req.m_posx = x;
        req.m_posy = y;
        GetModule<IRenderModule>()->AddPendingHitRequest("select actor", req);
    }
}