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
        void registerTexture(
            std::shared_ptr<RHIResource> out_abedlo,
            std::shared_ptr<RHIResource> out_normal,
            std::shared_ptr<RHIResource> out_diffuse,
            std::shared_ptr<RHIResource> out_specular,
            std::shared_ptr<RHIResource> out_ambient,
            std::shared_ptr<RHIResource> depth);

    private:
        std::shared_ptr<RHIAttachmentsSet> attachementSet;
        std::shared_ptr<RHIPipeline> pipeline;
        std::shared_ptr<RHIShader> vs;
        std::shared_ptr<RHIShader> ps;

        std::vector<std::shared_ptr<RHIDescriptorsSet>> descriptors_sets;
        std::vector<Entity> entitys;
    };
}