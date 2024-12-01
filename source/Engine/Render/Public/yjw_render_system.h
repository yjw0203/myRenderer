#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"

namespace yjw
{
    class RenderSystem
    {
    public:

        void initialize();
        void tick(float deltaTime);
        void shutdown();

        void AttachScene(class Scene* scene);
        void AttachUI(class rhi::ImGuiUI* ui);
        rpi::RPITexture GetSceneTexture();

        class RenderCamera* activeCamera = nullptr;
        class RenderCameraInputDispatcher* m_camera_dispatcher = nullptr;

        class Scene* m_scene = nullptr;
        class rhi::ImGuiUI* m_ui = nullptr;

        class Window* m_window = nullptr;

        float m_delta_time;

    };

}