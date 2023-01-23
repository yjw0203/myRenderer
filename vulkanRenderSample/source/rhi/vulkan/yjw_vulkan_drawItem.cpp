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
		bool build_finished = false;;
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		void build(DrawItem* item)
		{
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = g_context.queueFamilyIndices.graphicsFamily.value();
			vkCreateCommandPool(g_context.device, &poolInfo, nullptr, &g_context.commandPool);
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = g_context.commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;
			vkAllocateCommandBuffers(g_context.device, &allocInfo, &g_context.commandBuffer);

			renderPass = createRenderPass();
			pipelineLayout = createPipelineLayout();
			pipeline = createPipeline(pipelineLayout, renderPass);

			build_finished = true;
		}

		~BuildContext()
		{
			if (build_finished)
			{
				vkDestroyCommandPool(g_context.device, commandPool, nullptr);
				vkDestroyPipeline(g_context.device, pipeline, nullptr);
				vkDestroyPipelineLayout(g_context.device, pipelineLayout, nullptr);
				vkDestroyRenderPass(g_context.device, renderPass, nullptr);
			}
		}
	};

	std::map<int, BuildContext> drawitem_id_to_buildContext;
	void DrawItem::draw()
	{
		build();
		//add command buffer to list

	}

	void DrawItem::setRasterizationState(RasterizationState state)
	{
		if (rasterizationState == state)return;
		rasterizationState = state;
		dirty = true;
	}

	void DrawItem::build()
	{
		if (!dirty)
		{
			//prepare command buffer
			BuildContext& buildContext = drawitem_id_to_buildContext[id] = BuildContext();

			buildContext.build(this);

		}

	}

	DrawItem::DrawItem()
	{
		id = ++drawItem_id_allocator;
	}

	DrawItem::~DrawItem()
	{
		drawitem_id_to_buildContext.erase(id);
	}
}