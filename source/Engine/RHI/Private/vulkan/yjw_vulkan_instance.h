#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_instance.h"
#include "vulkan/vulkan.h"

namespace rhi
{
    extern PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
    extern PFN_vkCmdEndDebugUtilsLabelEXT   _vkCmdEndDebugUtilsLabelEXT;

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