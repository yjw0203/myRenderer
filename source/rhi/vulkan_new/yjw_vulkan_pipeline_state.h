#pragma once
#include "vulkan/vulkan.h"
#include "rhi/rhi/yjw_rhi_pipeline_state.h"
namespace rhi
{

    class VulkanPilelineStateManager
    {
    public:
        static VulkanPilelineStateManager& Get()
        {
            static bool initialized = false;
            static VulkanPilelineStateManager instance;
            if (!initialized)
            {
                instance.initializeRasterizationState();
                instance.initializeColorBlendState();
                initialized = true;
            }
            return instance;
        }
        VkPipelineRasterizationStateCreateInfo& getRasterizationState(RasterizationState state)
        {
            return rasterizationState[state];
        }

        VkPipelineColorBlendStateCreateInfo& getColorBlendState(ColorBlendState state)
        {
            return colorBlendState[state];
        }

    private:
        void initializeRasterizationState()
        {
            VkPipelineRasterizationStateCreateInfo rasterizer{};
            rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizer.depthClampEnable = VK_FALSE;
            rasterizer.rasterizerDiscardEnable = VK_FALSE;
            rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
            rasterizer.lineWidth = 1.0f;
            rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizer.depthBiasEnable = VK_FALSE;

            rasterizationState[RasterizationState::Rasterization_default] = rasterizer;
        }
        void initializeColorBlendState()
        {
            static VkPipelineColorBlendAttachmentState colorBlendAttachments[1];
            colorBlendAttachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachments[0].blendEnable = VK_FALSE;

            VkPipelineColorBlendStateCreateInfo colorBlending{};
            colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlending.logicOpEnable = VK_FALSE;
            colorBlending.logicOp = VK_LOGIC_OP_COPY;
            colorBlending.attachmentCount = 1;
            colorBlending.pAttachments = &colorBlendAttachments[0];
            colorBlending.blendConstants[0] = 0.0f;
            colorBlending.blendConstants[1] = 0.0f;
            colorBlending.blendConstants[2] = 0.0f;
            colorBlending.blendConstants[3] = 0.0f;

            colorBlendState[ColorBlendState::ColorBlend_default] = colorBlending;
        }

        VkPipelineRasterizationStateCreateInfo rasterizationState[RasterizationState::Rasterization_count];
        VkPipelineColorBlendStateCreateInfo colorBlendState[ColorBlendState::ColorBlend_count];
    };

}