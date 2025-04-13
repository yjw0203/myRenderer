#pragma once
#include "Engine/Engine/Private/Editor/MajorEditorInput.h"

namespace rhi
{
    class ImGuiUI;
}

namespace yjw
{
    class MajorEditor
    {
    public:
        void Startup();
        void Destroy();
        void Tick();

        void SelectActor(int actor_id);
        void UnselectActor();
    private:
        void OnClicked(float x, float y);
    private:
        MajorInputDispatcher m_input_dispatcher{};

        class World* m_world = nullptr;
        class Window* m_window = nullptr;
        class ViewProxy* m_view = nullptr;
        class ::rhi::ImGuiUI* m_ui;

        int m_select_actor_id{};
    };
}