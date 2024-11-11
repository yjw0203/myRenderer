#include "yjw_editor_ui.h"
#include "Engine/Framework/Scene/Scene.h"
#include <array>

namespace yjw
{
    void EditorUI::DecodeUI()
    {
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        //ImGui::SetNextWindowPos(ImVec2(50, 50));
        ImGui::SetNextWindowSize(ImVec2(1900,1080));
        //ImGui::Begin("Engine", nullptr, flags);
        //ShowMenuBar();
        //ShowWorldObject();
        //ShowMenu();
        //ShowActorList();
        //ShowViewport();
        //ImGui::End();
        
    }

    void EditorUI::ShowMenuBar()
    {
        ImGuiDockNodeFlags dock_flags = ImGuiDockNodeFlags_DockSpace;
        ImGuiWindowFlags   window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
            ImGuiConfigFlags_NoMouseCursorChange | ImGuiWindowFlags_NoBringToFrontOnFocus;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(main_viewport->WorkPos, ImGuiCond_Always);
        std::array<int, 2> window_size = std::array<int, 2>{1200, 1200};
        ImGui::SetNextWindowSize(ImVec2((float)window_size[0], (float)window_size[1]), ImGuiCond_Always);

        ImGui::SetNextWindowViewport(main_viewport->ID);

        static bool p_open = true;
        ImGui::Begin("Editor menu", &p_open, window_flags);
        /*
        ImGuiID main_docking_id = ImGui::GetID("Main Docking");
        if (ImGui::DockBuilderGetNode(main_docking_id) == nullptr)
        {
            ImGui::DockBuilderRemoveNode(main_docking_id);

            ImGui::DockBuilderAddNode(main_docking_id, dock_flags);
            ImGui::DockBuilderSetNodePos(main_docking_id,
                ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 18.0f));
            ImGui::DockBuilderSetNodeSize(main_docking_id,
                ImVec2((float)window_size[0], (float)window_size[1] - 18.0f));

            ImGuiID center = main_docking_id;
            ImGuiID left;
            ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f, nullptr, &left);

            ImGuiID left_other;
            ImGuiID left_file_content = ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.30f, nullptr, &left_other);

            ImGuiID left_game_engine;
            ImGuiID left_asset =
                ImGui::DockBuilderSplitNode(left_other, ImGuiDir_Left, 0.30f, nullptr, &left_game_engine);

            ImGui::DockBuilderDockWindow("World Objects", left_asset);
            ImGui::DockBuilderDockWindow("Components Details", right);
            ImGui::DockBuilderDockWindow("File Content", left_file_content);
            ImGui::DockBuilderDockWindow("Game Engine", left_game_engine);

            ImGui::DockBuilderFinish(main_docking_id);
        }

        ImGui::DockSpace(main_docking_id);
        */
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Reload Current Level"))
                {
                }
                if (ImGui::MenuItem("Save Current Level"))
                {
                }
                if (ImGui::BeginMenu("Debug"))
                {
                    if (ImGui::BeginMenu("Animation"))
                    {
                        if (ImGui::MenuItem("show skeleton"))
                        {
                            
                        }
                        if (ImGui::MenuItem("show bone name"))
                        {
                            
                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Camera"))
                    {
                        if (ImGui::MenuItem("show runtime info"))
                        {

                        }
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Game Object"))
                    {
                        if (ImGui::MenuItem("show bounding box"))
                        {
                            
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Exit"))
                {
                    exit(0);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
    void EditorUI::ShowWorldObject()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        if (!ImGui::Begin("World Objects", nullptr, window_flags))
        {
            ImGui::End();
            return;
        }

        if (ImGui::Selectable("www"))
        {
            
        }

        ImGui::End();
    }
    void EditorUI::ShowMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Item1"))
            {
                if (ImGui::MenuItem("test1"))
                {
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Item2"))
            {
                if (ImGui::MenuItem("test2"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void EditorUI::ShowActorList()
    {
        if (ImGui::TreeNode("ActorList"))
        {
            for (const Actor* actor : m_scene->GetActors())
            {
                ImGui::Text(actor->GetName());
            }
            ImGui::TreePop();
        }
    }

    void EditorUI::ShowViewport()
    {
        if (ImGui::TreeNode("Viewport"))
        {
            ImGui::Image(m_scene_texture, ImVec2(600, 600));
            ImGui::TreePop();
        }
    }


}