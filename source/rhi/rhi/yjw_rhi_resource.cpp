#include "yjw_rhi_resource.h"
#include "yjw_rhi_interface.h"

namespace rhi
{
    RHIResource::RHIResource(RHIResourceType type, int width, int height, int depthOrArraySize, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType)
        :rhiResourceDesc{ type , width, height,depthOrArraySize ,miplevels,format,usage,memoryType }
    {
        resourceLocation = IRHI::Get()->createResource(rhiResourceDesc);
    }
    RHIResource::~RHIResource()
    {
        IRHI::Get()->deleteResource(resourceLocation);
    }

    RHIBuffer::RHIBuffer(int size, RHIResourceUsage usage, RHIMemoryType memoryType)
        :RHIResource(RHIResourceType::buffer,
            size,
            1,
            1,
            1,
            RHIFormat::unknow,
            usage,
            memoryType)
    {
    }

    RHITexture2D::RHITexture2D(int width, int height, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType)
        :RHIResource(RHIResourceType::texture2D,
            width,
            height,
            1,
            miplevels,
            format,
            usage,
            memoryType)
    {

    }
}