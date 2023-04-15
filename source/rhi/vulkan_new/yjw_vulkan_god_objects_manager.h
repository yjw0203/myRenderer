#pragma once
#include "vulkan/vulkan.h"
#include "rhi/rhi/yjw_rhi_header.h"
#include <vector>
#include <optional>
#include "rhi/rhi/yjw_rhi_interface.h"

namespace rhi
{
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class VulkanGod
    {
    public:
        CreateInfo createInfo;
        VkInstance instance;
        VkPhysicalDevice gpu;
        VkDevice device;
        VkSurfaceKHR surface;

        //queue
        QueueFamilyIndices queueFamilyIndices;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        //command
        std::vector<VkCommandBuffer> commandBufferList;

        //swapchain
        VkSwapchainKHR swapchain;
        VkFormat swapchainImageFormat;
        VkExtent2D swapchainExtent;
        std::vector<VkImage> swapchainImages;
        std::vector<VkImageView> swapchainImageViews;
        uint32_t swapchainImageIndex;

        //signal
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;

        //debug
        bool isDebugMode = true;

        //other
        VkSampler defaultSampler;

        void initialize(CreateInfo info);
        void beginFrame();
        void endFrame(RHIResource* present_texture);
    };
    extern VulkanGod vulkanGod;

    VkRenderPass createRenderPass(std::vector<RHIResourceView*>& rtvs, RHIResourceView* dsv);
    void createDescriptorLayoutSets(VkDescriptorPool& descriptorPool, std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, std::vector<VkDescriptorSet>& descriptorSets, RHIShaderView* vs, RHIShaderView* ps);
    VkFramebuffer createFramebuffer(VkRenderPass renderPass, std::vector<RHIResourceView*>& rtvs, RHIResourceView* dsv, int width, int height);
    VkPipelineLayout createPipelineLayout(std::vector<VkDescriptorSetLayout>& descriptorSetLayouts);
    VkPipeline createPipeline(VkPipelineLayout& pipelineLayout,
        VkRenderPass& renderPass,
        RasterizationState rasterizationState,
        ColorBlendState colorBlendState,
        std::vector<VkPipelineShaderStageCreateInfo>& shaderStages);
    VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfo(RHIShaderView* shader, VkShaderStageFlagBits stage);
    void transitionImageLayout(VkCommandBuffer& commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

}