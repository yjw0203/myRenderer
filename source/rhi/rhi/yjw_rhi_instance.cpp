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
        delete m_impl;
    }

    RHIDevice* RHIInstance::CreateDevice()
    {
        return m_impl->CreateDevice();
    }

    void RHIInstance::DestroyDevice(RHIDevice* device)
    {
        m_impl->DestroyDevice(device);
    }
}