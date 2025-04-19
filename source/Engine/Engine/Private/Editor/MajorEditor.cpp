#include "Engine/Engine/Private/Editor/MajorEditor.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Private/Editor/yjw_editor_ui.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"

#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/Engine/Public/Window.h"

#include "Engine/Render/Public/RenderAPI.h"
namespace yjw
{
    void MajorEditor::Startup()
    {
        m_input_dispatcher.Register();
        m_input_dispatcher.SetOnClicked(std::bind(&MajorEditor::OnClicked, this, std::placeholders::_1, std::placeholders::_2));

        m_world = new World();
        m_ui = new EditorUI(m_world);
        m_window = new Window();

        m_view = rdCreateView(m_window->GetWindowHandle());
        rdAttachScene(m_view, m_world->GetScene());
        rdAttachUI(m_view, m_ui);

        m_world->GetLevel()->SpawnActor<MeshActor>("naxita", "naxita/naxita.mesh.ast");
        Actor* heita = m_world->GetLevel()->SpawnActor<MeshActor>("heita", "heita/heita.mesh.ast");

        Transform transform{};
        transform.m_location = glm::vec3(10, 10, 0);
        rdUpdateEntityTransform(m_world->GetScene(), heita->GetSceneEntity(), transform);
    }

    void MajorEditor::Destroy()
    {
        rdDestroyView(m_view);
        delete m_world;
        delete m_ui;
        delete m_window;
    }

    void MajorEditor::Tick()
    {
        std::vector<RdHitRequestStruct> proccessed_request;
        rdGetProcessedHitRequest(m_view, "select actor", proccessed_request);
        if (!proccessed_request.empty())
        {
            if (proccessed_request.back().m_result)
            {
                SelectActor(proccessed_request.back().m_result);
            }
            else
            {
                UnselectActor();
            }
            printf("%d %d %d %d\n", proccessed_request.back().m_result, proccessed_request.back().m_result, proccessed_request.back().m_result, proccessed_request.back().m_result);
        }

        rdDrawView(m_view);
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
            m_world->GetScene()->UpdateEntityRenderMask(actor->GetSceneEntity(), RdRenderMaskBits::highlight, true);
            m_select_actor_id = actor_id;
        }
    }

    void MajorEditor::UnselectActor()
    {
        Actor* actor = m_world->GetLevel()->GetActorById(m_select_actor_id);
        if (actor)
        {
            m_world->GetScene()->UpdateEntityRenderMask(actor->GetSceneEntity(), RdRenderMaskBits::highlight, false);
        }
    }

    void MajorEditor::OnClicked(float x, float y)
    {
        RdHitRequestStruct req{};
        req.m_posx = x;
        req.m_posy = y;
        rdAddPendingHitRequest(m_view, "select actor", req);
    }
}