#include "yjw_vulkan_rhi.h"

#include <stdexcept>
#include <iostream>

namespace yjw
{
    void VulkanComponents::createComponents()
    {
        createInstance();

    }

    void VulkanComponents::shutdownComponents()
    {
        shutdownInstance();

    }

    void VulkanComponents::createInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledExtensionCount = 0;
        createInfo.ppEnabledExtensionNames = nullptr;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }
    void VulkanComponents::createPhysicalDevice()
    {
        uint32_t gpuDeviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &gpuDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> gpuList(gpuDeviceCount);
        vkEnumeratePhysicalDevices(m_instance, &gpuDeviceCount, gpuList.data());
        m_gpu = gpuList[0];
    }
    void VulkanComponents::createDevice()
    {

    }
    void VulkanComponents::shutdownInstance()
    {
        vkDestroyInstance(m_instance, nullptr);
    }
    void VulkanComponents::shutdownPhysicalDevice()
    {

    }
    void VulkanComponents::shutdownDevice()
    {

    }


    void VulkanRHI::initialize()
    {
        m_components.createComponents();


    }
    
    void VulkanRHI::destory()
    {
        m_components.shutdownComponents();
    }
}