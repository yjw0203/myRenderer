#include "yjw_pso_adaptor.h"
#include "yjw_property_adaptor.h"
#include "yjw_shader_adaptor.h"
#include <cassert>

namespace vulkan
{
    PSOFactoryAdaptor::PSOFactoryAdaptor(const rhi::PSOInitConfig& initConfig)
    {
        PSOVertexBinding vertex_binding;
        vertex_binding.locations.resize(1);
        for (int i = 0; i < initConfig.vertex_binding.count; i++)
        {
            vertex_binding.locations[0].push_back(VertexLocation{ i,FormatAdaptor(initConfig.vertex_binding.vertex_formats[i]) });
        }
        frac.bind(&vertex_binding);

        PSORasterizationStateBinding rasterizationState_binding;
        rasterizationState_binding.cullMode = CullModeAdptor(initConfig.rasterization_state_binding.cullMode);
        rasterizationState_binding.polygonMode = PolygonModeAdptor(initConfig.rasterization_state_binding.polygonMode);
        rasterizationState_binding.frontFace = FrontFaceAdptor(initConfig.rasterization_state_binding.frontFace);
        rasterizationState_binding.lineWidth = 1.0f;
        frac.bind(&rasterizationState_binding);

        PSOAttachmentBinding attachment_binding;
        frac.bind(&attachment_binding);

        PSODescriptorLayoutBinding descriptor_layout_binding;
        frac.bind(&descriptor_layout_binding);

        for (int i = 0; i < initConfig.shader_binding.count; i++)
        {
            PSOShaderBinding shader_binding;
            shader_binding.shader = ShaderAdaptor(*initConfig.shader_binding.shader_entries[i].shader);
            shader_binding.entryName = initConfig.shader_binding.shader_entries[i].entryName;
            frac.bind(&shader_binding);
        }
    }
}