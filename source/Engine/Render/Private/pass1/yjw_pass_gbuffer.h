#pragma once
#include "../yjw_pass.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Model/Public/yjw_model.h"
#include "Engine/Engine/Public/Render/Scene.h"

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

        std::vector<RPIResourceBinding> resource_bindings;
        std::vector<RPIBuffer> uniformsBuffers;
        std::vector<RenderEntity> m_entitys;
    };
}