#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_layer.h"

namespace yjw
{
    using namespace rpi;
    class ImGuiPass
    {
    public:
        void Initialize();
        void Destroy();
        void Submit(RPIContext context);
        void LoadResource(rhi::ImGuiUI* ui);

    private:
        RPITexture m_sky_box{};

        RPIBuffer m_vertex_buffer{};
        RPIBuffer m_index_buffer{};
        RPIPipeline m_pipeline{};
        RPIShader m_vs{};
        RPIShader m_ps{};
        RPIResourceBinding m_resource_binding{};
        RPIPrimitiveBinding m_primitive_binding{};
        RPIResourceSet m_global_resource_set{};
        RPIResourceSet m_custom_vs_resource_set{};
        RPIResourceSet m_custom_ps_resource_set{};

        rhi::ImGuiUI* m_ui{};
        RHIImguiLayer* m_imgui_layer{};
    };
}