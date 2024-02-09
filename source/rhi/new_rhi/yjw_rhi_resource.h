#pragma once
#include "yjw_rhi_define.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"

namespace rhi
{
    enum class ResourceUsageBits;
    typedef int ResourceUsage;

    class RHIResourceCreation
    {
    public:
        ResourceType type;
        int width;
        int height;
        int depthOrArraySize;
        int miplevels;
        Format format;
        ResourceUsage usage;
        MemoryType memoryType;
    };

    typedef RHIHandle RHIResourceHandle;
}