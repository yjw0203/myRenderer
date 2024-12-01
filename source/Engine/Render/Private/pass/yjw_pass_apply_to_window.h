#pragma once
#include "../yjw_pass.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Model/Public/yjw_model.h"

namespace yjw
{
    using namespace rpi;
    class ApplyToWindowPass : public Pass
    {
    public:
        ApplyToWindowPass(RPIWindow window);
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(RPITexture apply_tex);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;
        RPIWindow m_window;

        RPIResourceBinding resourceBinding;
        RPIPrimitiveBinding m_primitive_binding;
        RPIBuffer vertex_buffer;

    };
}