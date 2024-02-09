#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_object.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"

namespace rhi
{
    struct DescriptorBinding
    {
        ShaderType shaderStage;
        int setId;
        int bindId;
        int slotId;
        DescriptorType descriptorType;
    };

    struct PSOVextexBinding
    {
        std::vector<Format> vertex_formats;
    };

    struct PSOAttachmentBinding
    {
        std::vector<Format> attachment_formats;
        Format depth_stencil_format;
    };

    struct PSODescriptorLayoutBinding
    {
        std::vector<DescriptorBinding> descriptor_bindings;
    };

    struct PSOShaderBinding
    {
        std::vector<ShaderEntry> shader_entries;
    };

    struct PSORasterizationStateBinding
    {
        PolygonMode polygonMode;
        CullMode cullMode;
        FrontFace frontFace;
    };

    struct PSOCreation
    {
        PSOVextexBinding vertex_binding;
        PSOAttachmentBinding attachment_binding;
        PSODescriptorLayoutBinding descriptor_layout_binding;
        PSOShaderBinding shader_binding;
        PSORasterizationStateBinding rasterization_state_binding;
    };

    typedef RHIHandle RHIPSOHandle;
}