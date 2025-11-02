#include "yjw_editor_ui.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include <array>
#include <string>
#include <windows.h>
#include <commdlg.h>
#include "Tools/ModelImporter/Public/ModelImporter.h"
#include "projectInfo.h"
#include "Engine/Engine/Private/Editor/MajorEditor.h"

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

void EditTransform(const glm::mat4x4& view_matrix, const glm::mat4x4& proj_matrix, Transform& transform, EditTransformMode mode)
{
    static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Z))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_X))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_C)) // r Key
        mCurrentGizmoOperation = ImGuizmo::SCALE;
    /*
    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;
        
    ImGui::InputFloat3("Tr", (float*)&transform.m_location, "%.3f", 3);
    ImGui::InputFloat3("Rt", (float*)&transform.m_rotate,"%.3f", 3);
    ImGui::InputFloat3("Sc", (float*)&transform.m_scale,"%.3f", 3);

    if (mCurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
            mCurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
            mCurrentGizmoMode = ImGuizmo::WORLD;
    }
    static bool useSnap(false);
    if (ImGui::IsKeyPressed(ImGuiKey_V))
        useSnap = !useSnap;
    ImGui::Checkbox("useSnap", &useSnap);
    ImGui::SameLine();
    static glm::vec3 snap{};
    switch (mCurrentGizmoOperation)
    {
    case ImGuizmo::TRANSLATE:
        //snap = config.mSnapTranslation;
        ImGui::InputFloat3("Snap", &snap.x);
        break;
    case ImGuizmo::ROTATE:
        //snap = config.mSnapRotation;
        ImGui::InputFloat("Angle Snap", &snap.x);
        break;
    case ImGuizmo::SCALE:
        //snap = config.mSnapScale;
        ImGui::InputFloat("Scale Snap", &snap.x);
        break;
    }
    */

    static bool useSnap(false);
    static glm::vec3 snap{};

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(viewport->WorkPos.x, viewport->WorkPos.y, io.DisplaySize.x, io.DisplaySize.y);

    glm::mat4x4 matrix = transform.getMatrix();
    ImGuizmo::Manipulate((float*)&view_matrix, (float*)&proj_matrix, mCurrentGizmoOperation, ImGuizmo::LOCAL, (float*)&matrix, NULL, useSnap ? &snap.x : NULL);

    if (ImGuizmo::IsUsing())
    {
        glm::vec3 translation, rotation, scale;
        ImGuizmo::DecomposeMatrixToComponents((float*)&matrix, (float*)&translation, (float*)&rotation, (float*)&scale);
        rotation = rotation / 180.0f * 3.14159265f;
        transform.m_location = translation;
        transform.m_rotate = glm::quat(rotation);
        transform.m_scale = scale;
    }
}


void EditorUI::DecodeUI()
{
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
    ImGui::Begin("Engine", nullptr, flags);
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::BeginChild("transform");
    if (m_edit_transform_ptr)
    {
        EditTransform(rdGetViewMatrix(), rdGetProjMatrix(), *m_edit_transform_ptr, EditTransformMode::translate);
    }
    ImGui::EndChild();
    ImGui::End();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2(500, 500));
    ImGui::Begin("option");
    ShowOption();
    ImGui::End();

    ImGui::PopStyleColor();
}

void EditorUI::ShowOption()
{
    if (ImGui::TreeNode("Option"))
    {
        static char input_text[256] = "";
        static char input_text1[256] = "";
        ImGui::InputText("Import Dir:", input_text, IM_ARRAYSIZE(input_text));
        ImGui::InputText("Import Name:", input_text1, IM_ARRAYSIZE(input_text1));
        if (ImGui::Button("Import Resource"))
        {
            std::string path = OpenFileDialog();
            std::string ast_path = input_text;
            std::string ast_name = input_text1;
            ImportAsset(ast_path, ast_name, path);
            /*
            if (path != "")
            {
                std::string ast_name = input_text;
                ImportModel(ast_name, path);
                m_world->GetLevel()->SpawnActor<MeshActor>("test", (ast_name + ".mesh.ast").c_str());
            }*/
        }
        ImGui::TreePop();
    }
}


