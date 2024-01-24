#include "yjw_property_adaptor.h"
#include <cassert>

namespace vulkan
{
    FormatAdaptor::operator VkFormat()
    {
        switch (payload)
        {
        case rhi::Format::unknow: return VkFormat::VK_FORMAT_UNDEFINED;
        case rhi::Format::R8G8B8A8_unorm: return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
        case rhi::Format::R8G8B8A8_snorm: return VkFormat::VK_FORMAT_R8G8B8A8_SNORM;
        case rhi::Format::R8G8B8A8_srgb: return VkFormat::VK_FORMAT_R8G8B8A8_SRGB;
        case rhi::Format::R8G8B8A8_sfloat: return VkFormat::VK_FORMAT_R8G8B8A8_SSCALED;
        case rhi::Format::R32G32B32_sfloat: return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
        case rhi::Format::R32G32B32A32_sfloat: return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
        case rhi::Format::R32G32_sfloat: return VkFormat::VK_FORMAT_R32G32_SFLOAT;
        case rhi::Format::D24_unorm_S8_uint: return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
        }
        assert(0);
    }

    ImageUsageFlagsAdptor::operator VkImageUsageFlags()
    {
        VkImageUsageFlags flag = 0;
        if ((int)rhi::ResourceUsageBits::allow_render_target & payload)
        {
            flag |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_depth_stencil & payload)
        {
            flag |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_unordered_access & payload)
        {
            flag |= VK_IMAGE_USAGE_STORAGE_BIT;
        }
        if (!((int)rhi::ResourceUsageBits::deny_shader_resource & payload))
        {
            flag |= VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_transfer_src & payload)
        {
            flag |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_transfer_dst & payload)
        {
            flag |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        return flag;
    }

    BufferUsageFlagsAdptor::operator VkBufferUsageFlags()
    {
        VkBufferUsageFlags flag = 0;
        if ((int)rhi::ResourceUsageBits::allow_unordered_access & payload)
        {
            flag |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }
        if (!((int)rhi::ResourceUsageBits::deny_shader_resource & payload))
        {
            flag |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_transfer_src & payload)
        {
            flag |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_transfer_dst & payload)
        {
            flag |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_vertex_buffer & payload)
        {
            flag |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_index_buffer & payload)
        {
            flag |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }
        if ((int)rhi::ResourceUsageBits::allow_indirect_buffer & payload)
        {
            flag |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
        }
        return flag;
    }

    MemoryPropertyFlagsAdptor::operator VkMemoryPropertyFlags()
    {
        VkMemoryPropertyFlags flag = 0;
        if (payload == rhi::MemoryType::default_)
        {
            flag |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        }
        else if (payload == rhi::MemoryType::readback)
        {
            flag |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        }
        else if (payload == rhi::MemoryType::upload)
        {
            flag |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        }
        return flag;
    }

    PolygonModeAdptor::operator VkPolygonMode()
    {
        switch (payload)
        {
        case rhi::PolygonMode::fill: return VkPolygonMode::VK_POLYGON_MODE_FILL;
        case rhi::PolygonMode::line: return VkPolygonMode::VK_POLYGON_MODE_LINE;
        case rhi::PolygonMode::point: return VkPolygonMode::VK_POLYGON_MODE_POINT;
        }
        assert(0);
    }

    CullModeAdptor::operator VkCullModeFlags()
    {
        switch (payload)
        {
        case rhi::CullMode::none: return VK_CULL_MODE_NONE;
        case rhi::CullMode::back: return VK_CULL_MODE_BACK_BIT;
        case rhi::CullMode::front: return VK_CULL_MODE_FRONT_BIT;
        case rhi::CullMode::front_and_back: return VK_CULL_MODE_FRONT_AND_BACK;
        }
        assert(0);
    }

    FrontFaceAdptor::operator VkFrontFace()
    {
        switch (payload)
        {
        case rhi::FrontFace::counter_clockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
        case rhi::FrontFace::clockwise: return VK_FRONT_FACE_CLOCKWISE;
        }
        assert(0);
    }
}