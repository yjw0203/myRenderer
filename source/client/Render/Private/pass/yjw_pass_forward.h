#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "Model/yjw_model.h"
#include "Private/yjw_scene.h"

namespace yjw
{
    using namespace rpi;
    class ForwardPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        void setData(float metallic,float roughness);
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(
            RPITexture out_abedlo,
            RPITexture depth);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        std::vector<RPIResourceBinding> resource_bindings;
        std::vector<RPIBuffer> uniformsBuffers;
        std::vector<RenderEntity> m_entitys;

        float m_metallic = 0.5;
        float m_roughness = 0.5;
    };
}