#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{

	class RPIDescriptorSetWriter
	{
	public:
		RPIDescriptorSetWriter(RPIDescriptorSet handle) : descriptorSet(handle) {}
		void setDescriptor(int setId, int slot, RPIDescriptor descriptor);
		void submit();
	private:
		RPIDescriptorSet descriptorSet;
		std::vector<rhi::DescriptorWrite> writeCommands;
	};

	class RPIAttachmentSetCreator
	{
	public:
		RPIAttachmentSetCreator(RPIPipeline psoHandle);
		void setColorAttachment(int slot, RPIDescriptor descriptor);
		void setDepthStencilAttachment(RPIDescriptor descriptor);
		RPIAttachmentSet create();
	private:
		rhi::RHIAttachmentSetCreation creation{};
	};

	class RPIAttachmentSetDestoryer
	{
	public:
		RPIAttachmentSetDestoryer(RPIAttachmentSet handle) : handle(handle) {}
		void destory();
	private:
		RPIAttachmentSet handle{};
	};
}