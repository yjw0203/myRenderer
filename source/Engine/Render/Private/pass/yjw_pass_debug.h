#pragma once
#include "../yjw_pass.h"
#include "rhi/rpi/yjw_rpi_header.h"
#include "Model/yjw_model.h"

namespace yjw
{
    using namespace rpi;
    class DebugPass : public Pass
    {
    public:
        virtual void buildPSO() override;
        virtual void setupData() override;
        virtual void recordCommand(RPIContext commandBuffer) override;
        virtual void submit() override;
        void registerTexture(RPITexture out_tex);
        void Clear();
        void AddLine(glm::vec3 point0, glm::vec3 point1);
    private:
        RPIRenderPass renderPass;
        RPIPipeline pipeline;
        RPIShader vs;
        RPIShader ps;

        RPIResourceBinding resourceBinding;
        RPIPrimitiveBinding m_primitive_binding;

        RPIBuffer vertex_buffer;

        const int m_max_line_count = 1000;

        std::vector<glm::vec3> m_lines;
        int m_line_count;
    };
}