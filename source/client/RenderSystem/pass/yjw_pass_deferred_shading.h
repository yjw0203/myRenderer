#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "client/RenderSystem/yjw_model.h"

namespace yjw
{
    using namespace rhi;
    class DeferredShadingPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPICommandBuffer commandBuffer) override;
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
        RPIAttachmentSet attachementSet;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        RPIDescriptorSet descriptors_set;

        RPIDescriptor in_abeldo;
        RPIDescriptor in_normal;
        RPIDescriptor in_depth;
        RPIDescriptor in_diffuse;
        RPIDescriptor in_specular;
        RPIDescriptor in_ambient;
        RPIDescriptor out_color;

        RPIBuffer vertex_buffer;

    };
}