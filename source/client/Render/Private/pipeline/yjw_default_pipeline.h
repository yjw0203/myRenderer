#pragma once
#include "../yjw_pipeline.h"
#include <map>
#include <string>
#include <memory>
#include "rhi/rpi/yjw_rpi_header.h"
#include "Private/yjw_pass.h"
#include "Private/editorUI/yjw_editor_ui.h"
#include "Private/pass/yjw_pass_forward.h"
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

        RPITexture output;
        RPIContext commandBuffer;
    private:
        struct Resource
        {
            RPITexture resource_handle;
            bool need_clear_pre_render = false;
        };

        std::shared_ptr<ForwardPass> forwardPass;

        std::map<std::string, Resource> texture_map;
        std::vector<std::shared_ptr<Pass> > passes;
        rhi::ImGuiUI* m_ui = nullptr;

    };
}