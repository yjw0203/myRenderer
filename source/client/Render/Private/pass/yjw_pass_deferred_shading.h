#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "Private/yjw_model.h"

namespace yjw
{
    using namespace rhi;
    class DeferredShadingPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(
            RPITexture in_abeldo,
            RPITexture in_normal,
            RPITexture in_diffuse,
            RPITexture in_specular,
            RPITexture in_ambient,
            RPITexture in_depth,
            RPITexture out_color);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        RPIResourceBinding resourceBinding;

        RPIBuffer vertex_buffer;

    };
}