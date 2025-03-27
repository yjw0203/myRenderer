#include "Engine/RHI/Private/vulkan/yjw_vulkan_device.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_command_queue.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_swap_chain.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_pipeline.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_shader.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_context.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_buffer.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_texture.h"
#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"

#include <vector>
#include <set>
#include <stdexcept>
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
        deviceFeatures.multiViewport = true;

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
            "VK_LAYER_KHRONOS_validation",
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
        m_immediately_command_list = new VulkanCommandList(this);


        /***************** to be delete ***************/
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(m_gpu, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 1.0f;
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(m_native_device, &samplerInfo, nullptr, &m_default_sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }

        VkDescriptorSetLayoutCreateInfo dsl_createInfo{};
        dsl_createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        dsl_createInfo.bindingCount = 0;
        dsl_createInfo.pBindings = nullptr;
        dsl_createInfo.flags = 0;
        dsl_createInfo.pNext = nullptr;
        vkCreateDescriptorSetLayout(m_native_device, &dsl_createInfo, nullptr, &m_default_descriptor_layout);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 0;
        poolInfo.pPoolSizes = nullptr;
        poolInfo.maxSets = 1;

        if (vkCreateDescriptorPool(m_native_device, &poolInfo, nullptr, &m_default_descriptor_pool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }


        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_default_descriptor_pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &m_default_descriptor_layout;

        VkResult hr;
        if ((hr = vkAllocateDescriptorSets(m_native_device, &allocInfo, &m_default_descriptor_set)) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
        /***************** to be delete ***************/

        GetParentInstance()->OnDeviceInit(this);
    }

    RHISwapChain* VulkanDevice::CreateSwapchain(void* window)
    {
        return new VulkanSwapChain(this, window);
    }

    RHIShader* VulkanDevice::CreateShaderByBinary(const void* binaryData, int size, const char* entryName,const ShaderReflect& reflect)
    {
        return new VulkanShader(this, binaryData, size, entryName, reflect);
    }

    RHIResourceSet* VulkanDevice::CreateResourceSet(int set_id, const ShaderReflect& reflect)
    {
        return new VulkanResourceSet(this, set_id, reflect);
    }

    RHIResourceBinding* VulkanDevice::CreateResourceBinding()
    {
        return new VulkanResourceBinding(this);
    }

    RHIRenderPipeline* VulkanDevice::CreateRenderPipeline(const RHIRenderPipelineDescriptor& renderPipelineDescriptor)
    {
        return new VulkanRenderPipeline(this, renderPipelineDescriptor);
    }

    RHIComputePipeline* VulkanDevice::CreateComputePipeline(const RHIComputePipelineDescriptor& computePipelineDescriptor)
    {
        return new VulkanComputePipeline(this, computePipelineDescriptor);
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
        GetParentInstance()->OnDeviceShutdown(this);

        delete m_immediately_command_list;
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

    VulkanCommandList* VulkanDevice::GetImmediaCommandList()
    {
        return m_immediately_command_list;
    }

    void VulkanDevice::WaitForIdle()
    {
        vkQueueWaitIdle(m_command_queue->GetGraphicsQueue());
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