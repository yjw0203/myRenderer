#pragma once
#include "../yjw_pass.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/RenderSystem/yjw_model.h"

namespace yjw
{
    using namespace rhi;
    class GBufferPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand() override;
        virtual void submit() override;
        void registerTexture(std::shared_ptr<RHIColorAttachment> out_abedlo, std::shared_ptr<RHIColorAttachment> out_normal, std::shared_ptr<RHIDepthStencilAttachment> depth);

    private:
        std::shared_ptr<RHIAttachmentsSet> attachementSet;
        std::shared_ptr<RHIPipeline> pipeline;
        std::shared_ptr<RHIShader> vs;
        std::shared_ptr<RHIShader> ps;

        std::shared_ptr<RHIUniformBuffer> camera_uniform;
        std::shared_ptr<RHIUniformBuffer> camera_pos_uniform;
        std::shared_ptr<RHIUniformBuffer> light_uniform;

        std::vector<std::shared_ptr<RHIDescriptorsSet>> descriptors_sets;
        std::vector<Entity> entitys;
    };
}