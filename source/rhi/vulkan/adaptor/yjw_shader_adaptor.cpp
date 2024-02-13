#include "yjw_shader_adaptor.h"
#include <cassert>

namespace vulkan
{
    VulkanShaderCreationAdaptor::VulkanShaderCreationAdaptor(const rhi::RHIShaderCreation& creation)
    {
        frac.path = creation.name;
    }
}