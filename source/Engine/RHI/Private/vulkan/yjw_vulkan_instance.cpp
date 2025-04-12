#include "Engine/RHI/Private/vulkan/yjw_vulkan_instance.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "vector"
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace rhi
{
    PFN_vkCmdBeginDebugUtilsLabelEXT _vkCmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT   _vkCmdEndDebugUtilsLabelEXT;

    std::vector<const char*> getRequiredExtensions(bool is_debug_mode)
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        return extensions;
    }

    VulkanInstance::VulkanInstance(RHIInstanceConfig config)
        :RHIInstanceImpl(config)
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

        std::vector<const char*> extensions = getRequiredExtensions(config.isDebugMode);
        createInfo.enabledExtensionCount = (uint32_t)extensions.size();
        createInfo.ppEnabledExtensionNames = extensions.data();

        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation",
        };
        if (config.isDebugMode)
        {
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_native_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }

        _vkCmdBeginDebugUtilsLabelEXT =
            (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(m_native_instance, "vkCmdBeginDebugUtilsLabelEXT");
        _vkCmdEndDebugUtilsLabelEXT =
            (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(m_native_instance, "vkCmdEndDebugUtilsLabelEXT");

        OnInstanceInit(this);
    }

    VulkanInstance::~VulkanInstance()
    {
        RHIInstanceImpl::~RHIInstanceImpl();
        vkDestroyInstance(m_native_instance,nullptr);
    }

    RHIDevice* VulkanInstance::CreateDevice()
    {
        return new VulkanDevice(this, GetConfig().isDebugMode);
    }

    VkInstance VulkanInstance::GetNativeInstance()
    {
        return m_native_instance;
    }
}