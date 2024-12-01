#pragma once
#include "../yjw_pass.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Model/Public/yjw_model.h"

namespace yjw
{
    using namespace rpi;
    class DrawTrianglePass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(RPITexture out_tex);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;
    };
}