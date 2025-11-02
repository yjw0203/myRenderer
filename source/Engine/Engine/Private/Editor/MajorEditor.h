#pragma once
#include "Engine/Engine/Private/Editor/MajorEditorInput.h"
#include "Engine/Math/Public/Transform.h"

namespace rhi
{
    class ImGuiUI;
}

enum class EditTransformMode
{
    none,
    translate,
    rotate,
    scale
};

class MajorEditor
{
public:
    void Startup();
    void Destroy();
    void Tick(float deltaTime);

    void SelectActor(int actor_id);
    void UnselectActor();
private:
    void OnMouseClicked(float x, float y);
    void OnMousePress(float x, float y);
    void OnMouseMove(float x, float y);
    void OnMouseRelease(float x, float y);
    void OnKeyPressedQ();
    void OnKeyPressedW();
    void OnKeyPressedE();
private:
    MajorInputDispatcher m_input_dispatcher{};

    class World* m_world = nullptr;
    class Window* m_window = nullptr;
    class RdView* m_view = nullptr;
    class EditorUI* m_ui;

    int m_select_actor_id{};
    EditTransformMode m_transform_mode{ EditTransformMode::translate };
    Transform m_select_actor_edit_transform{};
};
