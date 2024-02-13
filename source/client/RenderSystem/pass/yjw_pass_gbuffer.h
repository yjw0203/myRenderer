#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "client/RenderSystem/yjw_model.h"

namespace yjw
{
    using namespace rpi;
    class GBufferPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPICommandBuffer commandBuffer) override;
        virtual void submit() override;
        void registerTexture(
            RPITexture out_abedlo,
            RPITexture out_normal,
            RPITexture out_diffuse,
            RPITexture out_specular,
            RPITexture out_ambient,
            RPITexture depth);

    private:
        RPIAttachmentSet attachementSet;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        RPIDescriptor out_abeldo;
        RPIDescriptor out_normal;
        RPIDescriptor out_diffuse;
        RPIDescriptor out_specular;
        RPIDescriptor out_ambient;
        RPIDescriptor out_depth;

        std::vector<RPIDescriptorSet> descriptors_sets;
        std::vector<RPIBuffer> uniformsBuffers;
        std::vector<RPIDescriptor> uniformsBufferDescriptors;
        std::vector<Entity> entitys;
    };
}