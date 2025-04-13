#pragma once
#include <vector>
#include <string>
#define RHI_MAX_RENDER_TARGETS 16

namespace rhi
{
    class RHIShader;
    class RHIBuffer;
    class RHIBufferView;
    class RHITexture;
    class RHITextureView;

    enum class ERHIType
    {
        none,
        vulkan,
        d3d12,
        metal
    };

    enum RHILayerType
    {
        rhi_layer_imgui,
        rhi_layer_count
    };

    struct RHIInstanceConfig
    {
        ERHIType rhiType;
        bool isDebugMode;
        std::vector<RHILayerType> layers;
    };


    enum class RHIShaderType : char
    {
        vertex,
        fragment,
        compute,
        count
    };

    enum class RHIResourceType : char
    {
        unkown,
        buffer,
        texture1D,
        texture2D,
        texture3D,
        textureCube,
        texture2DArray
    };

    enum class RHIMemoryType : char
    {
        default_ = 1,
        upload = 2,
        readback = 3,
    };

    enum class RHIResourceUsageBits : int
    {
        none = 0,
        allow_render_target = 0x1,
        allow_depth_stencil = 0x2,
        allow_unordered_access = 0x4,
        deny_shader_resource = 0x8,
        allow_transfer_src = 0x10,
        allow_transfer_dst = 0x20,
        allow_vertex_buffer = 0x40,//only for buffer
        allow_index_buffer = 0x80,//only for buffer
        allow_indirect_buffer = 0x100 //only for buffer
    };
    typedef int RHIResourceUsage;

    enum class RHIFormat : char
    {
        unknow = 0,
        R8G8B8A8_unorm,
        R8G8B8A8_snorm,
        R8G8B8A8_srgb,
        R8G8B8A8_sfloat,
        R8G8B8A8_uint,
        R8G8B8A8_sint,
        B8G8R8A8_srgb,
        B8G8R8A8_unorm,
        R32_sfloat,
        R32G32_sfloat,
        R32G32B32_sfloat,
        R32G32B32A32_sfloat,
        R32G32B32A32_uint,
        R32G32B32A32_sint,
        D24_unorm_S8_uint
    };

    enum class RHICompareOp : char
    {
        compare_op_never,
        compare_op_less,
        compare_op_equal,
        compare_op_less_or_equal,
        compare_op_greater,
        compare_op_not_equal,
        compare_op_greater_or_equal,
        compare_op_always,
        compare_op_count
    };

    enum class RHIStencilOp : char 
    {
        stencil_op_keep,
        stencil_op_zero,
        stencil_op_replace,
        stencil_op_increment_and_clamp,
        stencil_op_decrement_and_clamp,
        stencil_op_invert,
        stencil_op_increment_and_wrap,
        stencil_op_decrement_and_wrap,
        stencil_op_count
    };

    enum class RHICullMode : char
    {
        cull_mode_none,
        cull_mode_front,
        cull_mode_back,
        cull_mode_front_and_back,
        cull_mode_count
    };

    enum RHIPrimitiveTopology : char
    {
        primitive_topology_point_list,
        primitive_topology_line_list,
        primitive_topology_line_strip,
        primitive_topology_triangle_list,
        primitive_topology_triangle_strip,
        primitive_topology_triangle_fan,
    };

    enum RHIBlendFactor : char 
    {
        blend_factor_zero,
        blend_factor_one,
        blend_factor_src_color,
        blend_factor_one_minus_src_color,
        blend_factor_dst_color,
        blend_factor_one_minus_dst_color,
        blend_factor_src_alpha,
        blend_factor_one_minus_src_alpha,
        blend_factor_dst_alpha,
        blend_factor_one_minus_dst_alpha,
        blend_factor_constant_color,
        blend_factor_one_minus_constant_color,
        blend_factor_constant_alpha,
        blend_factor_one_minus_constant_alpha,
    };

    enum RHIBlendOp : char
    {
        blend_op_add,
        blend_op_subtract,
        blend_op_reverse_subtract,
        blend_op_min,
        blend_op_max
    };

    typedef char RHIColorComponentFlags;

    enum RHIColorComponentFlagBits : char
    {
        color_component_r = 0x00000001,
        color_component_g = 0x00000002,
        color_component_b = 0x00000004,
        color_component_a = 0x00000008,
    };

    struct RHIBufferDescriptor
    {
        RHIResourceType resourceType;
        RHIMemoryType memoryType;
        int width;
        RHIResourceUsage usage;
    };

    struct RHIBufferViewDescriptor
    {
        RHIBuffer* buffer;
        int offset;
        int width;
        RHIFormat format;
    };

    struct RHITextureDescriptor
    {
        RHIResourceType resourceType;
        RHIFormat format;
        int width;
        int height;
        int depthOrArraySize;
        int miplevels;
        RHIResourceUsage usage;
        RHIMemoryType memoryType;
    };

    struct RHITextureViewDescriptor
    {
        RHIResourceType resourceType;
        RHITexture* texture;
        RHIFormat format;
        int baseMip{ 0 };
        int mipCount{ 1 };
        int baseLayer{ 0 };
        int layerCount{ 1 };
    };

    struct RHISamplerDescriptor
    {

    };

    struct RHIColorBlendState
    {
        bool                   blendEnable{};
        RHIBlendFactor         srcColorBlendFactor{};
        RHIBlendFactor         dstColorBlendFactor{};
        RHIBlendOp             colorBlendOp{};
        RHIBlendFactor         srcAlphaBlendFactor{};
        RHIBlendFactor         dstAlphaBlendFactor{};
        RHIBlendOp             alphaBlendOp{};
        RHIColorComponentFlags colorWriteMask = color_component_r | color_component_g | color_component_b | color_component_a;
    };

    struct RHIRenderPassDescriptor
    {
        RHITextureView* colorAttachments[RHI_MAX_RENDER_TARGETS] = {};
        int colorAttachmentCount = 0;
        RHITextureView* depthStencilAttachment = nullptr;
    };

    struct RHIStencilOpState 
    {
        RHIStencilOp failOp;
        RHIStencilOp passOp;
        RHIStencilOp depthFailOp;
        RHICompareOp compareOp;
        int          compareMask;
        int          writeMask;
        int          reference;
    };

    struct RHIDepthStencilState
    {
        bool              depthTestEnable;
        bool              depthWriteEnable;
        RHICompareOp      depthCompareOp;
        bool              depthBoundsTestEnable;
        bool              stencilTestEnable;
        RHIStencilOpState front;
        RHIStencilOpState back;
        float             minDepthBounds;
        float             maxDepthBounds;
    };

    struct RHIRasterizationState
    {
        RHIRasterizationState()
        {
            cull_mode = RHICullMode::cull_mode_back;
        };
        RHICullMode cull_mode;
    };

    struct RHIRenderPipelineDescriptor
    {
        RHIRenderPipelineDescriptor()
        {
            primitiveTopology = RHIPrimitiveTopology::primitive_topology_triangle_list;
        };
        RHIColorBlendState color_blend_state[RHI_MAX_RENDER_TARGETS] = {};
        RHIDepthStencilState depth_stencil_state{};
        RHIRasterizationState rasterization_state{};
        RHIPrimitiveTopology primitiveTopology{};
        RHIShader* vs{};
        RHIShader* ps{};
    };

    struct RHIComputePipelineDescriptor
    {
        RHIShader* cs{};
    };
}