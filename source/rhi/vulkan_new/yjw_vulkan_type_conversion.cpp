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
            case RHIFormat::R8G8B8A8_snorm: return VK_FORMAT_R8G8B8A8_SNORM;
            case RHIFormat::R8G8B8A8_srgb: return VK_FORMAT_R8G8B8A8_SRGB;
            case RHIFormat::R32G32B32_sfloat: return VK_FORMAT_R32G32B32_SFLOAT;
            case RHIFormat::R32G32B32A32_sfloat: return VK_FORMAT_R32G32B32A32_SFLOAT;
            case RHIFormat::R32G32_sfloat: return VK_FORMAT_R32G32_SFLOAT;
            case RHIFormat::D24_unorm_S8_uint: return VK_FORMAT_D24_UNORM_S8_UINT;
            }
            return VK_FORMAT_UNDEFINED;
        }

        VkShaderStageFlags convertShaderStageFlag(RHIShaderType stage)
        {
            switch (stage)
            {
            case RHIShaderType::vertex_shader: return VK_SHADER_STAGE_VERTEX_BIT;
            case RHIShaderType::pixel_shader: return VK_SHADER_STAGE_FRAGMENT_BIT;
            case RHIShaderType::compute_shader: return VK_SHADER_STAGE_COMPUTE_BIT;
            }
            return VK_SHADER_STAGE_ALL;
        }

        VkImageUsageFlags convertImageResourceUsage(RHIResourceUsage rhiUsage)
        {
            VkImageUsageFlags flag = 0;
            if (RHIResourceUsageBits::allow_render_target & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            }
            if (RHIResourceUsageBits::allow_depth_stencil & rhiUsage)
            {
                flag |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
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

        VkBufferUsageFlags convertBufferResourceUsage(RHIResourceUsage rhiUsage)
        {
            VkBufferUsageFlags flag = 0;
            if (RHIResourceUsageBits::allow_unordered_access & rhiUsage)
            {
                flag |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            }
            if (!(RHIResourceUsageBits::deny_shader_resource & rhiUsage))
            {
                flag |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            }
            if (RHIResourceUsageBits::allow_transfer_src & rhiUsage)
            {
                flag |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            }
            if (RHIResourceUsageBits::allow_transfer_dst & rhiUsage)
            {
                flag |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            }
            if (RHIResourceUsageBits::allow_vertex_buffer & rhiUsage)
            {
                flag |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            }
            if (RHIResourceUsageBits::allow_index_buffer & rhiUsage)
            {
                flag |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            }
            if (RHIResourceUsageBits::allow_indirect_buffer & rhiUsage)
            {
                flag |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
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
                flag |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            }
            return flag;
        }

        VkImageLayout convertResourceState(RHIResourceState rhiResourceState)
        {
            switch (rhiResourceState)
            {
            case undefine:return VK_IMAGE_LAYOUT_UNDEFINED;
            case render_target:return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case depth_stencil_write:return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case depth_stencil_read:return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case shader_resource_read:return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case transfer_src:return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case transfer_dst:return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case present_src:return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            }
            return VK_IMAGE_LAYOUT_UNDEFINED;
        }
    }
}