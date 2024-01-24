#pragma once
#include "yjw_rhi_define.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_object.h"

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
        Format* vertex_formats;
        int count;
    };

    struct PSOAttachmentBinding
    {
        Format* attachment_formats;
        int count;
        Format depth_stencil_format;
    };

    struct PSODescriptorLayoutBinding
    {
        DescriptorBinding* descriptor_bindings;
        int count;
    };

    struct PSOShaderBinding
    {
        ShaderEntry* shader_entries;
        int count;
    };

    struct PSORasterizationStateBinding
    {
        PolygonMode polygonMode;
        CullMode cullMode;
        FrontFace frontFace;
    };

    struct PSOInitConfig
    {
        PSOVextexBinding vertex_binding;
        PSOAttachmentBinding attachment_binding;
        PSODescriptorLayoutBinding descriptor_layout_binding;
        PSOShaderBinding shader_binding;
        PSORasterizationStateBinding rasterization_state_binding;
    };

    class PSO : public RHIObject
    {
    public:
    };
}