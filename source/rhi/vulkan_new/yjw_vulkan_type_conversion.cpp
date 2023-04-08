#include "vulkan/vulkan.h"
#include "rhi/rhi/yjw_rhi_resource.h"
#include "yjw_vulkan_type_conversion.h"

namespace rhi
{
    namespace VulkanConverter
    {
        VkFormat convertFormat(RHIFormat rhiFormat)
        {
            switch (rhiFormat)
            {
            case RHIFormat::unknow: return VK_FORMAT_UNDEFINED;
            case RHIFormat::R8G8B8A8_unorm: return VK_FORMAT_R8G8B8A8_UNORM;
            }
            return VK_FORMAT_UNDEFINED;
        }

        VkImageUsageFlags convertResourceUsage(RHIResourceUsage rhiUsage)
        {
            VkImageUsageFlags flag = 0;
            if (RHIResourceUsageBits::allow_render_target & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }
            if (RHIResourceUsageBits::allow_depth_stencil & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            }
            if (RHIResourceUsageBits::allow_unordered_access & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_STORAGE_BIT;
            }
            if (!(RHIResourceUsageBits::deny_shader_resource & rhiUsage))
            {
                flag |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
            }
            if (RHIResourceUsageBits::allow_transfer_src & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            }
            if (RHIResourceUsageBits::allow_transfer_dst & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            }
            return flag;
        }

        VkMemoryPropertyFlags convertMemoryProperty(RHIMemoryType rhiMemoryType)
        {
            VkMemoryPropertyFlags flag = 0;
            if (rhiMemoryType == RHIMemoryType::default_)
            {
                flag |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            }
            else if(rhiMemoryType == RHIMemoryType::readback)
            {
                flag |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            }
            else if (rhiMemoryType == RHIMemoryType::upload)
            {
                flag |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
            }
            return flag;
        }
    }
}