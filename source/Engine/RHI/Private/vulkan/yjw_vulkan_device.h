#pragma once
#include "Engine/RHI/Public/rhi/yjw_rhi_header.h"
#include "Engine/RHI/Private/vulkan/yjw_vulkan_instance.h"
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
        virtual RHIShader* CreateShaderByBinary(const void* binaryData, int size, const char* entryName, const ShaderReflect& reflect) override;
        virtual RHIResourceSet* CreateResourceSet(int set_id, const ShaderReflect& reflect) override;
        virtual RHIRenderPipeline* CreateRenderPipeline(const RHIRenderPipelineDescriptor& renderPipelineDescriptor) override;
        virtual RHIComputePipeline* CreateComputePipeline(const RHIComputePipelineDescriptor& computePipelineDescriptor) override;
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

        virtual void WaitForFence(RHIFence* fence) override;
        virtual void SetGlobalResourceSetLayout(int set_id, const ShaderReflect& reflect) override;
        ShaderReflect* GetGlobalResourceSetLayout(int set_id);
        void WaitForIdle();
    private:
        VulkanInstance* m_parent_instance = nullptr;
        VkPhysicalDevice m_gpu = nullptr;
        VkDevice m_native_device = nullptr;
        QueueFamilyIndices m_queue_family_indices{};
        class VulkanCommandQueue* m_command_queue = nullptr;
        class VulkanCommandList* m_immediately_command_list = nullptr;
        
        std::unordered_map<int, ShaderReflect> m_global_resource_set_layout;

    public:
        VkSampler m_default_sampler = nullptr;
        VkDescriptorSetLayout m_default_descriptor_layout = nullptr;
        VkDescriptorPool m_default_descriptor_pool = nullptr;
        VkDescriptorSet m_default_descriptor_set = nullptr;
    };

    class VulkanDeviceObject
    {
    public:
        VulkanDeviceObject(VulkanDevice* device);
        virtual ~VulkanDeviceObject();
        VulkanDevice* GetDevice();
    private:
        VulkanDevice* m_parent_device = nullptr;
    };
}