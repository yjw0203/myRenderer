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
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(
            RPITexture out_abedlo,
            RPITexture out_normal,
            RPITexture out_diffuse,
            RPITexture out_specular,
            RPITexture out_ambient,
            RPITexture depth);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        RPITextureView out_abeldo;
        RPITextureView out_normal;
        RPITextureView out_diffuse;
        RPITextureView out_specular;
        RPITextureView out_ambient;
        RPITextureView out_depth;

        std::vector<RPIResourceBinding> resource_bindings;
        std::vector<RPIBuffer> uniformsBuffers;
        std::vector<RPIBufferView> uniformsBufferDescriptors;
        std::vector<Entity> entitys;
    };
}