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
        RPIBufferView cameraUniformDescriptor;
        RPIBuffer lightUniform;
        RPIBufferView lightUniformDescriptor;
        RPIBuffer optionUniform;
        RPIBufferView optionUniformDescriptor;
    };

    extern GlobalResourceStore g_resource_store;
}