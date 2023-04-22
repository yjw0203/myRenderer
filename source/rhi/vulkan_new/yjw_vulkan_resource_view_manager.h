#pragma once

#include "rhi/rhi/yjw_rhi_resource_view.h"
#include "vulkan/vulkan.h"
namespace rhi
{
    class VulkanResourceViewLocation : public RHIResourceViewLocation
    {
        friend class VulkanResourceViewManager;
    public:
        const VkImageView* getVkImageView();
    private:
        RHIResourceType type;
        VkImageView view;
    };

    class VulkanResourceViewManager
    {
    public:
        static VulkanResourceViewManager& Get()
        {
            static VulkanResourceViewManager instance;
            return instance;
        }

        RHIResourceViewLocation* createResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format);
        void deleteResourceView(RHIResourceViewLocation*& location);
    };
}