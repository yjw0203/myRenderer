#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "Private/yjw_model.h"
#include "RHI/externs/imgui/yjw_rhi_imgui_layer.h"
#include "RHI/externs/imgui/yjw_rhi_imgui_window.h"

namespace yjw
{
    using namespace rpi;
    class DrawImGuiPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(RPITexture out_tex);
        void setData(ImGuiUI* ui);

    private:
        RPIRenderPass renderPass;
        RHIImguiLayer* m_imgui_layer = nullptr;
        ImGuiUI* m_ui = nullptr;
    };
}