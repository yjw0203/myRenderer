#include "yjw_pso_adaptor.h"
#include "yjw_property_adaptor.h"
#include "yjw_shader_adaptor.h"
#include <cassert>
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include <algorithm>

namespace vulkan
{
    PSOCreationAdaptor::PSOCreationAdaptor(rhi::PSOCreation& creation)
    {
        PSOVertexBinding vertex_binding;
        vertex_binding.locations.resize(1);
        for (int i = 0; i < creation.vertex_binding.vertex_formats.size(); i++)
        {
            vertex_binding.locations[0].push_back(VertexLocation{ i,FormatAdaptor(creation.vertex_binding.vertex_formats[i]) });
        }
        frac.bind(&vertex_binding);

        PSORasterizationStateBinding rasterizationState_binding;
        rasterizationState_binding.cullMode = CullModeAdptor(creation.rasterization_state_binding.cullMode);
        rasterizationState_binding.polygonMode = PolygonModeAdptor(creation.rasterization_state_binding.polygonMode);
        rasterizationState_binding.frontFace = FrontFaceAdptor(creation.rasterization_state_binding.frontFace);
        rasterizationState_binding.lineWidth = 1.0f;
        frac.bind(&rasterizationState_binding);

        PSOAttachmentBinding attachment_binding{};
        for (int i = 0; i < creation.attachment_binding.attachment_formats.size(); i++)
        {
            attachment_binding.color_attachments.push_back(AttachmentBinding{ FormatAdaptor(creation.attachment_binding.attachment_formats[i]) });
        }
        attachment_binding.has_depth_stencil_attachment = false;
        if (creation.attachment_binding.depth_stencil_format != rhi::Format::unknow)
        {
            attachment_binding.has_depth_stencil_attachment = true;
            attachment_binding.depth_stencil_attachment = AttachmentBinding{ FormatAdaptor(creation.attachment_binding.depth_stencil_format) };
        }
        frac.bind(&attachment_binding);

        PSODescriptorLayoutBinding descriptor_layout_binding;
        int max_set_id = 0;
        for (rhi::DescriptorBinding& binding : creation.descriptor_layout_binding.descriptor_bindings)
        {
            max_set_id = std::max(max_set_id, binding.setId);
        }
        descriptor_layout_binding.bindings.resize(max_set_id + 1);
        for (rhi::DescriptorBinding& binding : creation.descriptor_layout_binding.descriptor_bindings)
        {
            descriptor_layout_binding.bindings[binding.setId].push_back(DescriptorSetLayoutBinding(binding.slotId, DescriptorTypeAdaptor(binding.descriptorType), ShaderTypeAdaptor(binding.shaderStage)));
        }
        frac.bind(&descriptor_layout_binding);

        VkPipelineDepthStencilStateCreateInfo depthStencilState{};
        depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilState.depthTestEnable = creation.depth_stencil_state_binding.depthTest;
        depthStencilState.depthWriteEnable = creation.depth_stencil_state_binding.depthWrite;
        depthStencilState.depthCompareOp = CompareOptionAdptor(creation.depth_stencil_state_binding.depthCompareOption);
        frac.bind(depthStencilState);

        for (int i = 0; i < creation.shader_binding.shader_entries.size(); i++)
        {
            PSOShaderBinding shader_binding;
            shader_binding.shaderType = ShaderTypeAdaptor(creation.shader_binding.shader_entries[i].shaderType);
            shader_binding.shader = HandleCast<VulkanShader>(creation.shader_binding.shader_entries[i].shader);
            shader_binding.entryName = creation.shader_binding.shader_entries[i].entryName;
            frac.bind(&shader_binding);
        }
    }
}