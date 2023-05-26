#pragma once
#include <d3d12.h>
#include <string>
#include <vector>
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
        unknow = 0,
        R8G8B8A8_unorm,
        R8G8B8A8_snorm,
        R8G8B8A8_srgb,
        R32G32B32_sfloat,
        R32G32_sfloat,
        D24_unorm_S8_uint
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
        allow_vertex_buffer = 0x40,//only for buffer
        allow_index_buffer = 0x80,//only for buffer
        allow_indirect_buffer = 0x100 //only for buffer
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
        RHIResource() {};
        RHIResource(RHIResourceType type, int width, int height, int depthOrArraySize, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType);
        virtual ~RHIResource();
        RHIResourceLocation* resourceLocation = nullptr;
        const RHIResourceDesc& getDesc() { return rhiResourceDesc; }
    private:
        RHIResourceDesc rhiResourceDesc;
    };

    class RHIBuffer : public RHIResource
    {
    public:
        RHIBuffer(int size, RHIResourceUsage usage, RHIMemoryType memoryType);
        RHIBuffer(int size, RHIResourceUsage usage, RHIMemoryType memoryType, void* data);
    };

    class RHITexture2D : public RHIResource
    {
    public:
        RHITexture2D(int width,int height, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType);
    };

    class RHITexture2DFromFile : public RHIResource
    {
    public:
        RHITexture2DFromFile(const char* filePath);
    };

    int sizeofFormat(RHIFormat rhiFormat);

    class VertexLayout
    {
    public:
        int getTotalStride()
        {
            int sum = 0;
            for (RHIFormat format : layouts)
            {
                sum += sizeofFormat(format);
            }
            return sum;
        }
        VertexLayout& reset()
        {
            layouts.clear();
            return *this;
        }
        VertexLayout& push(RHIFormat format)
        {
            layouts.push_back(format);
            return *this;
        }
        std::vector<RHIFormat> layouts;
    };
    
}