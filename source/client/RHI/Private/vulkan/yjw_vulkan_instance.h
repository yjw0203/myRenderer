#pragma once
#include "Public/RHI/rhi/yjw_rhi_instance.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    class VulkanInstance : public RHIInstanceImpl
    {
    public:
        VulkanInstance(RHIInstanceConfig config);
        ~VulkanInstance();
        virtual RHIDevice* CreateDevice() override;
        VkInstance GetNativeInstance();
    private:
        VkInstance m_native_instance = nullptr;

    };
}