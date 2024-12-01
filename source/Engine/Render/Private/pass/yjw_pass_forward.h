#pragma once
#include "../yjw_pass.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Model/Public/yjw_model.h"
#include "Engine/Render/Private/yjw_scene.h"
#include "Engine/Engine/Public/Framework/Scene/Scene.h"

namespace yjw
{
    using namespace rpi;
    class ForwardPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        void setBoneMatrices(const std::vector<Matrix4x4>& matrices);
        void setData();
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(
            RPITexture out_abedlo,
            RPITexture depth);

        void attachScene(Scene* scene);

    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        std::vector<RenderEntity> m_entitys;

        float m_metallic = 0.5;
        float m_roughness = 0.5;

        RPIBuffer m_bone_matrices;
        const int m_max_bone_count = 10000;

        Scene* m_scene = nullptr;
    };
}