#include "yjw_rpi_drawer.h"
#include "rhi/new_rhi/yjw_rhi_header.h"
namespace rpi
{
	void RPIDrawer::cmdBeginPass()
	{
		rhi::GpuDevice->cmdBeginPass(commandBuffer, pipeline, attachmentSet);
	}
	void RPIDrawer::cmdEndPass()
	{
		rhi::GpuDevice->cmdEndPass(commandBuffer);
	}
	void RPIDrawer::cmdBindDescriptorSet(RPIDescriptorSet bindDescriptorSet)
	{
		rhi::GpuDevice->cmdBindDescriptorSet(commandBuffer, bindDescriptorSet);
	}
	void RPIDrawer::cmdBindVertexBuffer(RPIResource buffer)
	{
		rhi::GpuDevice->cmdBindVertexBuffer(commandBuffer, buffer);
	}
	void RPIDrawer::cmdBindIndexBuffer(RPIResource buffer)
	{
		rhi::GpuDevice->cmdBindIndexBuffer(commandBuffer, buffer);
	}
	void RPIDrawer::cmdDrawIndex(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
	{
		rhi::GpuDevice->cmdDrawIndex(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}
	void RPIDrawer::cmdDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
	{
		rhi::GpuDevice->cmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
	}
}