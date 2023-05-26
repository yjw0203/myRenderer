#pragma once
#include "../yjw_pass.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/RenderSystem/yjw_model.h"

namespace yjw
{
    using namespace rhi;
    class DeferredShadingPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand() override;
        virtual void submit() override;
        void registerTexture(std::shared_ptr<RHIShaderResourceTexture> in_abeldo, std::shared_ptr<RHIShaderResourceTexture> in_normal, std::shared_ptr<RHIShaderResourceTexture> in_depth, std::shared_ptr<RHIColorAttachment> out_color);

    private:
        std::shared_ptr<RHIAttachmentsSet> attachementSet;
        std::shared_ptr<RHIPipeline> pipeline;
        std::shared_ptr<RHIShader> vs;
        std::shared_ptr<RHIShader> ps;

        std::shared_ptr<RHIUniformBuffer> camera_uniform;
        std::shared_ptr<RHIUniformBuffer> light_uniform;
        std::shared_ptr<RHIUniformBuffer> camera_pos_uniform;
        std::shared_ptr<RHIUniformBuffer> screen_uniform;

        std::shared_ptr<RHIDescriptorsSet> descriptors_set;

        std::shared_ptr<RHIShaderResourceTexture> in_abeldo;
        std::shared_ptr<RHIShaderResourceTexture> in_normal;
        std::shared_ptr<RHIShaderResourceTexture> in_depth;

        std::unique_ptr<RHIBuffer> vertex_buffer;

    };
}