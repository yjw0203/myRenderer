#pragma once
#include <d3d12.h>
namespace rhi
{
    enum RHIResourceType
    {
        buffer,
        texture1D,
        texture2D,
        texture3D
    };
    enum RHIFormat
    {
        unknow,
        R8G8B8A8_unorm
    };
    enum RHIResourceUsageBits
    {
        none = 0,
        allow_render_target = 0x1,
        allow_depth_stencil = 0x2,
        allow_unordered_access = 0x4,
        deny_shader_resource = 0x8,
        allow_transfer_src = 0x10,
        allow_transfer_dst = 0x20,
    };
    typedef int RHIResourceUsage;

    enum class RHIMemoryType
    {
        default_ = 1,
        upload = 2,
        readback = 3,
    };

    enum RHIResourceState
    {
        undefine,
        render_target,
        depth_stencil_write,
        depth_stencil_read,
        shader_resource_read,
        transfer_src,
        transfer_dst,
        present_src
    };

    struct RHIResourceDesc
    {
        RHIResourceType type;
        int width;
        int height;
        int depthOrArraySize;
        int miplevels;
        RHIFormat format;
        RHIResourceUsage usage;
        RHIMemoryType memoryType;
    };

    class RHIResourceLocation{};

    class RHIResource
    {
    public:
        RHIResource(RHIResourceType type, int width, int height, int depthOrArraySize, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType);
        virtual ~RHIResource();
        const RHIResourceDesc rhiResourceDesc;
        RHIResourceLocation* resourceLocation = nullptr;
    };

    class RHIBuffer : public RHIResource
    {
    public:
        RHIBuffer(int size, RHIResourceUsage usage, RHIMemoryType memoryType);
    };

    class RHITexture2D : public RHIResource
    {
    public:
        RHITexture2D(int width,int height, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType);
    };
    
}