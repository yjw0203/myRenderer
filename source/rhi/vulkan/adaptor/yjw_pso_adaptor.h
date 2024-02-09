#pragma once
#include "rhi/new_rhi/yjw_rhi_pso.h"
#include "rhi/vulkan/yjw_vulkan_pso.h"
namespace vulkan
{
    class PSOCreationAdaptor
    {
    public:
        PSOCreationAdaptor(const rhi::PSOCreation& creation);
        operator VulkanPSOCreation& () { return frac; }
    private:
        VulkanPSOCreation frac;
    };
}