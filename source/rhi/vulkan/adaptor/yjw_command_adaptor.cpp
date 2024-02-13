#include "yjw_resource_adaptor.h"
#include "yjw_command_adaptor.h"
#include <cassert>

namespace vulkan
{
    RHICommandBufferCreationAdaptor::operator VulkanCommandBufferCreation()
    {
        return VulkanCommandBufferCreation{};
    }
}