#include "yjw_editor_ui.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include <array>
#include <string>
#include <windows.h>
#include <commdlg.h>
#include "Tools/ModelImporter/Public/ModelImporter.h"
#include "projectInfo.h"

namespace yjw
{
    std::string OpenFileDialog() {
        OPENFILENAME ofn;       // common dialog box structure
        char szFile[260];       // buffer for file name
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileName(&ofn)) {
            return std::string(ofn.lpstrFile);
        }
        return "";
    }

    void EditorUI::DecodeUI()
    {
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize(ImVec2(500,500));
        ImGui::Begin("Engine", nullptr, flags);
        ShowOption();
        ImGui::End();
        
    }

    void EditorUI::ShowOption()
    {
        if (ImGui::TreeNode("Option"))
        {
            static char input_text[256] = "";
            ImGui::InputText("Import Dir:", input_text, IM_ARRAYSIZE(input_text));
            if (ImGui::Button("Import Resource"))
            {
                std::string path = OpenFileDialog();
                if (path != "")
                {
                    std::string ast_name = input_text;
                    ImportModel(ast_name, path);
                    m_world->GetLevel()->SpawnActor<MeshActor>("test", (ast_name + ".mesh.ast").c_str());
                }
            }
            ImGui::TreePop();
        }
    }


}