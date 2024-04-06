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
        R32G32B32_sfloat,
        R32G32B32A32_sfloat,
        R32G32_sfloat,
        D24_unorm_S8_uint
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
        RHITextureView* colorAttachments[RHI_MAX_RENDER_TARGETS];
        int colorAttachmentCount;
        RHITextureView* depthStencilAttachment;
    };

    struct RHIDepthStencilState
    {

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
        std::string vs_entry;
        RHIShader* ps{};
        std::string ps_entry;
    };
}