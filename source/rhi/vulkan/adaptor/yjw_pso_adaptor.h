#pragma once
#include "rhi/new_rhi/yjw_rhi_pso.h"
#include "rhi/vulkan/yjw_vulkan_pso.h"
namespace vulkan
{
    class PSOFactoryAdaptor
    {
    public:
        PSOFactoryAdaptor(const rhi::PSOInitConfig& initConfig);
        operator PSOFactory& () { return frac; }
    private:
        PSOFactory frac;
    };
}