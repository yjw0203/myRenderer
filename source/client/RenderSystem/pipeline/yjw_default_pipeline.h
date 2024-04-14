#pragma once
#include "../yjw_pipeline.h"
#include <map>
#include <string>
#include <memory>
#include "rhi/rpi/yjw_rpi_header.h"
#include "client/RenderSystem/yjw_pass.h"
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

        std::map<std::string, Resource> texture_map;
        std::vector<std::shared_ptr<Pass> > passes;

    };
}