#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "Model/yjw_model.h"

namespace yjw
{
    using namespace rhi;
    class ToneMappingPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(
            RPITexture in_color,
            RPITexture out_color);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        RPIResourceBinding resourceBinding;
        RPIPrimitiveBinding m_primitive_binding;

        RPIBuffer vertex_buffer;

    };
}