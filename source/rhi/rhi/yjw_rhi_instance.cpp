#include "yjw_rhi_instance.h"
#include "RHI/vulkan/yjw_vulkan_instance.h"

namespace rhi
{
    RHIInstance::RHIInstance(RHIInstanceConfig config)
    {
        m_impl = new VulkanInstance(config);
    }

    RHIInstance::~RHIInstance()
    {
        m_impl->release();
    }

    RHIDevice* RHIInstance::CreateDevice()
    {
        return m_impl->CreateDevice();
    }
}