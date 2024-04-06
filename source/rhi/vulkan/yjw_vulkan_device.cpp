#include "yjw_vulkan_device.h"
#include <vector>
#include <set>
#include <stdexcept>
#include "yjw_vulkan_command_queue.h"
#include "yjw_vulkan_swap_chain.h"
#include "yjw_vulkan_pipeline.h"
#include "yjw_vulkan_shader.h"
#include "yjw_vulkan_context.h"
#include "yjw_vulkan_buffer.h"
#include "yjw_vulkan_texture.h"
namespace rhi
{
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            //vkGetPhysicalDeviceSurfaceSupportKHR(device, i, VK_G(VkSurfaceKHR), &presentSupport);
            presentSupport = true;

            if (presentSupport) {
                indices.presentFamily = i;
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }

    VulkanDevice::VulkanDevice(VulkanInstance* vkInstance, bool isDebugMode)
        :m_parent_instance(vkInstance)
    {
        uint32_t gpuDeviceCount = 0;
        vkEnumeratePhysicalDevices(vkInstance->GetNativeInstance(), &gpuDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> gpuList(gpuDeviceCount);
        vkEnumeratePhysicalDevices(vkInstance->GetNativeInstance(), &gpuDeviceCount, gpuList.data());
        m_gpu = gpuList[0];
        m_queue_family_indices = findQueueFamilies(m_gpu);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { m_queue_family_indices.graphicsFamily.value(), m_queue_family_indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        if (isDebugMode) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }
        VkResult re = vkCreateDevice(m_gpu, &createInfo, nullptr, &m_native_device);
        if (re != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        m_command_queue = new VulkanCommandQueue(this, m_queue_family_indices);
    }

    RHISwapChain* VulkanDevice::CreateSwapchain(void* window)
    {
        return new VulkanSwapChain(this, window);
    }

    RHIShader* VulkanDevice::CreateShaderByBinaryUrl(const char* url)
    {
        return new VulkanShader(this, url, true);
    }

    RHIRenderPipeline* VulkanDevice::CreateRenderPipeline(const RHIRenderPipelineDescriptor& renderPipelineDescriptor)
    {
        return new VulkanRenderPipeline(this, renderPipelineDescriptor);
    }

    RHIRenderPass* VulkanDevice::CreateRenderPass(const RHIRenderPassDescriptor& renderPassDescriptor)
    {
        return new VulkanRenderPass(this, renderPassDescriptor);
    }

    RHIBuffer* VulkanDevice::CreateBuffer(const RHIBufferDescriptor& bufferDescriptor)
    {
        return new VulkanBuffer(this, bufferDescriptor);
    }

    RHITexture* VulkanDevice::CreateTexture(const RHITextureDescriptor& tetureDescriptor)
    {
        return new VulkanTexture(this, tetureDescriptor);
    }

    RHIBufferView* VulkanDevice::CreateBufferView(const RHIBufferViewDescriptor& viewDescriptor)
    {
        return new VulkanBufferView(this, viewDescriptor);
    }

    RHITextureView* VulkanDevice::CreateTextureView(const RHITextureViewDescriptor& viewDescriptor)
    {
        return new VulkanTextureView(this, viewDescriptor);
    }

    RHIContext* VulkanDevice::CreateContext()
    {
        return new VulkanContext(this);
    }

    VulkanDevice::~VulkanDevice()
    {
        delete m_command_queue;
        vkDestroyDevice(m_native_device, nullptr);
    }

    VkPhysicalDevice VulkanDevice::GetGpu()
    {
        return m_gpu;
    }

    VkDevice VulkanDevice::GetNativeDevice()
    {
        return m_native_device;
    }

    VulkanInstance* VulkanDevice::GetParentInstance()
    {
        return m_parent_instance;
    }

    QueueFamilyIndices VulkanDevice::GetQueueFamilyIndices()
    {
        return m_queue_family_indices;
    }

    VulkanCommandQueue* VulkanDevice::GetCommandQueue()
    {
        return m_command_queue;
    }

    VulkanDeviceObject::VulkanDeviceObject(VulkanDevice* device)
        :m_parent_device(device)
    {
        
    }

    VulkanDeviceObject::~VulkanDeviceObject()
    {
        
    }

    VulkanDevice* VulkanDeviceObject::GetDevice()
    {
        return m_parent_device;
    }
}