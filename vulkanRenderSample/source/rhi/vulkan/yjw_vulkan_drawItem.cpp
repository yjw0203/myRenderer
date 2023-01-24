#include "../yjw_rhi.h"
#include "vulkan/vulkan.h"
#include "yjw_vulkan_context.h"
#include "yjw_vulkan_functions.h"
#include <map>
#include <stdexcept>
namespace rhi
{
	static int drawItem_id_allocator = 0;
	class BuildContext
	{
	public:
		~BuildContext()
		{
			if (build_finished)
			{
				vkDestroyCommandPool(g_context.device, commandPool, nullptr);
				vkDestroyPipeline(g_context.device, pipeline, nullptr);
				vkDestroyPipelineLayout(g_context.device, pipelineLayout, nullptr);
				vkDestroyRenderPass(g_context.device, renderPass, nullptr);
				vkDestroyFramebuffer(g_context.device, frameBuffer, nullptr);
			}
		}

		bool build_finished = false;;
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer[3];
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		VkFramebuffer frameBuffer;
		void build(DrawItem* item)
		{
			createObject(item);
			recordCommand(item);

			build_finished = true;
		}
	private:
		void createObject(DrawItem* item)
		{
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = g_context.queueFamilyIndices.graphicsFamily.value();
			vkCreateCommandPool(g_context.device, &poolInfo, nullptr, &commandPool);
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 3;
			vkAllocateCommandBuffers(g_context.device, &allocInfo, commandBuffer);

			renderPass = createRenderPass();
			pipelineLayout = createPipelineLayout();
			pipeline = createPipeline(pipelineLayout, renderPass);
			frameBuffer = createFramebuffer(renderPass, g_context.swapchainImageViews.data() + g_context.swapchainImageIndex, 1, g_context.swapchainExtent.width, g_context.swapchainExtent.height);
		}

		void recordCommand(DrawItem* item)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			vkBeginCommandBuffer(commandBuffer[0], &beginInfo);
			vkBeginCommandBuffer(commandBuffer[1], &beginInfo);
			vkBeginCommandBuffer(commandBuffer[2], &beginInfo);

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = frameBuffer;
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = g_context.swapchainExtent;

			VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(commandBuffer[2], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(commandBuffer[2], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)g_context.swapchainExtent.width;
			viewport.height = (float)g_context.swapchainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(commandBuffer[2], 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = g_context.swapchainExtent;
			vkCmdSetScissor(commandBuffer[2], 0, 1, &scissor);

			vkCmdDraw(commandBuffer[2], 3, 1, 0, 0);
			vkCmdEndRenderPass(commandBuffer[2]);

			vkEndCommandBuffer(commandBuffer[0]);
			vkEndCommandBuffer(commandBuffer[1]);
			vkEndCommandBuffer(commandBuffer[2]);
		}

	};

	void DrawItem::draw()
	{
		build();
		//add command buffer to list
		g_context.commandBufferList.push_back(buildContext->commandBuffer[0]);
		g_context.commandBufferList.push_back(buildContext->commandBuffer[1]);
		g_context.commandBufferList.push_back(buildContext->commandBuffer[2]);

	}

	void DrawItem::setRasterizationState(RasterizationState state)
	{
		if (rasterizationState == state)return;
		rasterizationState = state;
		dirty = true;
	}

	void DrawItem::build()
	{
		//if (dirty)
		{
			//prepare command buffer
			delete buildContext;
			buildContext = new BuildContext();

			buildContext->build(this);
			dirty = false;
		}

	}

	DrawItem::DrawItem()
	{
		id = ++drawItem_id_allocator;
	}

	DrawItem::~DrawItem()
	{
		delete buildContext;
	}
}