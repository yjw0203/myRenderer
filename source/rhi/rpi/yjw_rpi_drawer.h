#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{
	class RPIDrawer
	{
	public:
		RPIDrawer(RPIPipeline pipeline, RPICommandBuffer commandBuffer, RPIAttachmentSet attachmentSet) 
			:pipeline(pipeline), commandBuffer(commandBuffer), attachmentSet(attachmentSet){}
		void cmdBeginPass();
		void cmdEndPass();
		void cmdBindDescriptorSet(RPIDescriptorSet bindDescriptorSet);
		void cmdBindAttachmentSet(RPIAttachmentSet bindAttachmentSet);
		void cmdBindVertexBuffer(RPIResource buffer);
		void cmdBindIndexBuffer(RPIResource buffer);
		void cmdDrawIndex(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance);
		void cmdDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
	private:
		RPIPipeline pipeline;
		RPICommandBuffer commandBuffer;
		RPIAttachmentSet attachmentSet;

		//bind resouarce
		RPIDescriptorSet descriptorSet;
		RPIResource vertexBuffer;
		RPIResource indexBuffer;
	};
}