#pragma once
#include "rhi/rhi/yjw_rhi_header.h"
#include <memory>

namespace yjw
{
    using namespace rhi;
    class GlobalResourceStore
    {
    public:
        void initializeResource();
        void updateCameraData();
        void updateLightData();

    public:
        std::shared_ptr<RHIUniformBuffer> cameraUniform;
        std::shared_ptr<RHIUniformBuffer> lightUniform;
        std::shared_ptr<RHIUniformBuffer> optionUniform;
    };

    extern GlobalResourceStore g_resource_store;
}