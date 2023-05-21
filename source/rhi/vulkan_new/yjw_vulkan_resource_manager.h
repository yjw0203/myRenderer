#pragma once

#include "rhi/rhi/yjw_rhi_resource.h"
#include "vulkan/vulkan.h"
namespace rhi
{
    typedef struct VulkanResourceDesc
    {
        RHIResourceType type;
        union {
            VkBufferCreateInfo bufferInfo;
            VkImageCreateInfo imageInfo;
        };
    }VulkanResourceDesc;

    class VulkanResourceLocation : public RHIResourceLocation
    {
        friend class VulkanResourceManager;
    public:
        const VulkanResourceDesc& getDesc();
        const VkBuffer* getVkBuffer();
        const VkImage* getVkImage();
        const VkDeviceMemory* getVkDeviceMemory();
    private:

        union {
            VkImage image;
            VkBuffer buffer;
        };

        VkDeviceMemory memory;
        VulkanResourceDesc desc;
    };

    class VulkanResourceManager
    {
    public:
        static VulkanResourceManager& Get()
        {
            static VulkanResourceManager instance;
            return instance;
        }

        RHIResourceLocation* createResource(const RHIResourceDesc& desc);
        void deleteResource(RHIResourceLocation*& location);
    };

    class VulkanResourceCopyer
    {
    public:
        static void copyResource(VkCommandBuffer commandBuffer, RHIResource* src, RHIResource* dst);
    private:
        static void copyBufferToTexture2D(VkCommandBuffer commandBuffer, RHIResource* src, RHIResource* dst);
    };

    class VulkanResourceWriter
    {
    public:
        static void writeResourceImmidiately(RHIResource* resource, void* data, int size, int offset);
    private:
        static void writeBufferImmidiately(RHIResource* resource, void* data, int size, int offset);
    };
}