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
        B8G8R8A8_srgb,
        B8G8R8A8_unorm,
        R32G32B32_sfloat,
        R32G32B32A32_sfloat,
        R32G32_sfloat,
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
        RHITexture* texture;
        RHIFormat format;
    };

    struct RHISamplerDescriptor
    {

    };

    struct RHIColorBlendState
    {

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

    };

    struct RHIRenderPipelineDescriptor
    {
        RHIColorBlendState color_blend_state{};
        RHIDepthStencilState depth_stencil_state{};
        RHIRasterizationState rasterization_state{};
        RHIShader* vs{};
        RHIShader* ps{};
        std::vector<RHIFormat> vertex_layouts;
    };
}