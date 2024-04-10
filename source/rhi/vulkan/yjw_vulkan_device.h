#pragma once
#include "RHI/rhi/yjw_rhi_header.h"
#include "yjw_vulkan_instance.h"
#include "vulkan/vulkan.h"
#include <optional>

namespace rhi
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class VulkanDevice : public RHIDevice
    {
    public:
        VulkanDevice(VulkanInstance* vkInstance, bool isDebugMode);
        ~VulkanDevice();

        virtual RHIContext* CreateContext() override;
        virtual RHISwapChain* CreateSwapchain(void* window) override;
        virtual RHIShader* CreateShaderByBinaryUrl(const char* url) override;
        virtual RHIRenderPipeline* CreateRenderPipeline(const RHIRenderPipelineDescriptor& renderPipelineDescriptor) override;
        virtual RHIRenderPass* CreateRenderPass(const RHIRenderPassDescriptor& renderPassDescriptor) override;
        virtual RHIBuffer* CreateBuffer(const RHIBufferDescriptor& bufferDescriptor) override;
        virtual RHITexture* CreateTexture(const RHITextureDescriptor& tetureDescriptor) override;
        virtual RHIBufferView* CreateBufferView(const RHIBufferViewDescriptor& viewDescriptor) override;
        virtual RHITextureView* CreateTextureView(const RHITextureViewDescriptor& viewDescriptor) override;

        VkPhysicalDevice GetGpu();
        VkDevice GetNativeDevice();
        VulkanInstance* GetParentInstance();
        QueueFamilyIndices GetQueueFamilyIndices();
        class VulkanCommandQueue* GetCommandQueue();
        class VulkanCommandList* GetImmediaCommandList();

        void WaitForIdle();
    private:
        VulkanInstance* m_parent_instance = nullptr;
        VkPhysicalDevice m_gpu = nullptr;
        VkDevice m_native_device = nullptr;
        QueueFamilyIndices m_queue_family_indices{};
        class VulkanCommandQueue* m_command_queue = nullptr;
        class VulkanCommandList* m_immediately_command_list = nullptr;

    /***************** to be delete ***************/
    public:
        VkSampler m_default_sampler = nullptr;
    /***************** to be delete ***************/
    };

    class VulkanDeviceObject
    {
    public:
        VulkanDeviceObject(VulkanDevice* device);
        ~VulkanDeviceObject();
        VulkanDevice* GetDevice();
    private:
        VulkanDevice* m_parent_device = nullptr;
    };
}