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
        m_input_dispatcher.SetOnClicked(std::bind(&MajorEditor::OnMouseClicked, this, std::placeholders::_1, std::placeholders::_2));
        m_input_dispatcher.SetOnMousePress(std::bind(&MajorEditor::OnMousePress, this, std::placeholders::_1, std::placeholders::_2));
        m_input_dispatcher.SetOnMouseMove(std::bind(&MajorEditor::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2));
        m_input_dispatcher.SetOnMouseRelease(std::bind(&MajorEditor::OnMouseRelease, this, std::placeholders::_1, std::placeholders::_2));
        m_input_dispatcher.SetOnQ(std::bind(&MajorEditor::OnKeyPressedQ, this));
        m_input_dispatcher.SetOnW(std::bind(&MajorEditor::OnKeyPressedW, this));
        m_input_dispatcher.SetOnE(std::bind(&MajorEditor::OnKeyPressedE, this));

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
        transform.m_rotate = glm::quat(glm::vec3(2, 2, 2));
        heita->SetTransform(transform);
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

        if (m_select_actor_id && m_transform_mode != EditTransformMode::none)
        {
            Actor* actor = m_world->GetLevel()->GetActorById(m_select_actor_id);
            if (actor)
            {
                actor->SetTransform(m_select_actor_edit_transform);
                rdUpdateEntityTransform(m_world->GetScene(), actor->GetSceneEntity(), m_select_actor_edit_transform);
            }
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
            m_select_actor_edit_transform = actor->GetTransform();
            m_ui->SetEditTransformPtr(&m_select_actor_edit_transform);
            rdUpdateEntityRenderMask(m_world->GetScene(), actor->GetSceneEntity(), RdRenderMaskBits::highlight, true);
            m_select_actor_id = actor_id;
        }
    }

    void MajorEditor::UnselectActor()
    {
        Actor* actor = m_world->GetLevel()->GetActorById(m_select_actor_id);
        if (actor)
        {
            //m_ui->SetEditTransformPtr(nullptr);
            rdUpdateEntityRenderMask(m_world->GetScene(), actor->GetSceneEntity(), RdRenderMaskBits::highlight, false);
        }
    }

    void MajorEditor::OnMouseClicked(float x, float y)
    {
        
    }

    void MajorEditor::OnMousePress(float x, float y)
    {
        RdHitRequestStruct req{};
        req.m_posx = x;
        req.m_posy = y;
        rdAddPendingHitRequest(m_view, "select actor", req);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse)
        {

        }
        else
        {

        }
    }

    void MajorEditor::OnMouseMove(float x, float y)
    {

    }

    void MajorEditor::OnMouseRelease(float x, float y)
    {

    }

    void MajorEditor::OnKeyPressedQ()
    {
        m_transform_mode = EditTransformMode::translate;
    }

    void MajorEditor::OnKeyPressedW()
    {
        m_transform_mode = EditTransformMode::rotate;
    }

    void MajorEditor::OnKeyPressedE()
    {
        m_transform_mode = EditTransformMode::scale;
    }
}