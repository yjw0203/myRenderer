#pragma once
#include "../yjw_pipeline.h"
#include <map>
#include <string>
#include <memory>
#include "rhi/rhi/yjw_rhi_header.h"
#include "client/RenderSystem/yjw_pass.h"
namespace yjw
{
    using namespace rhi;
    class DefaultPipeline : Pipeline
    {
    public:
        virtual void initializeResource() override;
        virtual void config() override;
        virtual void render() override;

        std::shared_ptr<RHIResource> output = nullptr;
    private:
        struct Resource
        {
            std::shared_ptr<RHIResource> resource_handle;
            bool need_clear_pre_render = false;
        };

        std::map<std::string, Resource> texture_map;
        std::vector<std::shared_ptr<Pass> > passes;

    };
}