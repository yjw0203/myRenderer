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

        class RenderCamera* activeCamera = nullptr;
        class RenderCameraInputDispatcher* m_camera_dispatcher = nullptr;

        float m_delta_time;

    };

}