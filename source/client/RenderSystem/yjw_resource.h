#pragma once
#include "rhi/rpi/yjw_rpi_header.h"
#include <memory>

namespace yjw
{
    using namespace rpi;
    class GlobalResourceStore
    {
    public:
        void initializeResource();
        void updateCameraData();
        void updateLightData();

    public:
        RPIBuffer cameraUniform;
        RPIBuffer lightUniform;
        RPIBuffer optionUniform;
    };

    extern GlobalResourceStore g_resource_store;
}