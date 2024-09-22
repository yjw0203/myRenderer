#pragma once
#include "../yjw_pipeline.h"
#include <map>
#include <string>
#include <memory>
#include "rhi/rpi/yjw_rpi_header.h"
#include "Private/yjw_pass.h"
#include "Private/editorUI/yjw_editor_ui.h"
#include "Private/pass/yjw_pass_forward.h"
#include "Private/pass/yjw_pass_debug.h"
namespace yjw
{
    using namespace rpi;
    struct DefaultPipelineConfig
    {
        RPIWindow window;
    };

    class DefaultPipeline : Pipeline
    {
    public:
        virtual void initializeResource() override;
        void config(DefaultPipelineConfig config);
        virtual void render() override;

        void ClearDebugLine();
        void AddDebugLine(glm::vec3 point0, glm::vec3 point1);

        RPITexture output;
        RPIContext commandBuffer;

        std::shared_ptr<ForwardPass> forwardPass;
    private:
        struct Resource
        {
            RPITexture resource_handle;
            bool need_clear_pre_render = false;
        };


        std::shared_ptr<DebugPass> debugPass;

        std::map<std::string, Resource> texture_map;
        std::vector<std::shared_ptr<Pass> > passes;
        rhi::ImGuiUI* m_ui = nullptr;

    };
}