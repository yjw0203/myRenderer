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

    DescriptorTypeAdaptor::operator VulkanDescriptorType()
    {
        switch (payload)
        {
        case rhi::DescriptorType::uniform_buffer: return VulkanDescriptorType::uniform_buffer;
        case rhi::DescriptorType::shader_resource_texture: return VulkanDescriptorType::shader_resource_texture;
        case rhi::DescriptorType::depth_stencil: return VulkanDescriptorType::depth_stencil;
        case rhi::DescriptorType::render_target: return VulkanDescriptorType::render_target;
        case rhi::DescriptorType::unordered_access: return VulkanDescriptorType::unordered_access;
        }
        assert(0);
    }

    DescriptorTypeAdaptor::operator VkDescriptorType()
    {
        switch (payload)
        {
        case rhi::DescriptorType::uniform_buffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case rhi::DescriptorType::shader_resource_texture: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }
        assert(0);
    }

    ShaderTypeAdaptor::operator VkShaderStageFlags()
    {
        switch (payload)
        {
        case rhi::ShaderType::vertex_shader: return VK_SHADER_STAGE_VERTEX_BIT;
        case rhi::ShaderType::pixel_shader: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case rhi::ShaderType::compute_shader: return VK_SHADER_STAGE_COMPUTE_BIT;
        }
        assert(0);
    }

    ShaderTypeAdaptor::operator VkShaderStageFlagBits()
    {
        switch (payload)
        {
        case rhi::ShaderType::vertex_shader: return VK_SHADER_STAGE_VERTEX_BIT;
        case rhi::ShaderType::pixel_shader: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case rhi::ShaderType::compute_shader: return VK_SHADER_STAGE_COMPUTE_BIT;
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

    ResouraceStateAdptor::operator VkImageLayout()
    {
        switch (payload)
        {
        case rhi::RHIResourceState::undefine: return VK_IMAGE_LAYOUT_UNDEFINED;
        case rhi::RHIResourceState::render_target: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case rhi::RHIResourceState::depth_stencil_write: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case rhi::RHIResourceState::depth_stencil_read: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        case rhi::RHIResourceState::shader_resource_read: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case rhi::RHIResourceState::transfer_src: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case rhi::RHIResourceState::transfer_dst: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case rhi::RHIResourceState::present_src: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        }
        assert(0);
    }
}