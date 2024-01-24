#pragma once
#include "rhi/new_rhi/yjw_rhi_resource.h"
#include "vulkan/vulkan.h"

namespace vulkan
{
    class FormatAdaptor
    {
    public:
        FormatAdaptor(rhi::Format format) :payload(format) {}
        operator VkFormat();
    private:
        rhi::Format payload;
    };

    class ImageUsageFlagsAdptor
    {
    public:
        ImageUsageFlagsAdptor(rhi::ResourceUsage usage) :payload(usage) {}
        operator VkImageUsageFlags();
    private:
        rhi::ResourceUsage payload;
    };

    class BufferUsageFlagsAdptor
    {
    public:
        BufferUsageFlagsAdptor(rhi::ResourceUsage usage) :payload(usage) {}
        operator VkBufferUsageFlags();
    private:
        rhi::ResourceUsage payload;
    };

    class MemoryPropertyFlagsAdptor
    {
    public:
        MemoryPropertyFlagsAdptor(rhi::MemoryType memoryType) :payload(memoryType) {}
        operator VkMemoryPropertyFlags();
    private:
        rhi::MemoryType payload;
    };

    class PolygonModeAdptor
    {
    public:
        PolygonModeAdptor(rhi::PolygonMode polygonMode) :payload(polygonMode) {}
        operator VkPolygonMode();
    private:
        rhi::PolygonMode payload;
    };

    class CullModeAdptor
    {
    public:
        CullModeAdptor(rhi::CullMode cullMode) :payload(cullMode) {}
        operator VkCullModeFlags();
    private:
        rhi::CullMode payload;
    };

    class FrontFaceAdptor
    {
    public:
        FrontFaceAdptor(rhi::FrontFace frontFace) :payload(frontFace) {}
        operator VkFrontFace();
    private:
        rhi::FrontFace payload;
    };
}