#pragma once
#include "yjw_rhi_define.h"

namespace rhi
{
    enum class ResourceUsageBits;
    typedef int ResourceUsage;

    struct Buffer
    {
        void* payload;
    };

    struct Texture
    {
        void* payload;
    };

    struct ResourceInitConfig
    {
        ResourceType type;
        int width;
        int height;
        int depthOrArraySize;
        int miplevels;
        Format format;
        ResourceUsage usage;
        MemoryType memoryType;
    };

    class Resource : public RHIObject
    {
    public:
        Resource(const ResourceInitConfig& inInitConfig, void* inPayload) : initConfig(inInitConfig) { setPayload(inPayload); };
    public:
        const ResourceInitConfig initConfig;
    };
}