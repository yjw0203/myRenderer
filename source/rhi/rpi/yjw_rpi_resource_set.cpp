#include "yjw_rpi_resource_set.h"

namespace rpi
{
	void RPIDescriptorSetWriter::setDescriptor(int setId, int slot, RPIDescriptor descriptor)
	{
		writeCommands.push_back(rhi::DescriptorWrite{ setId,slot,descriptor });
	}

	void RPIDescriptorSetWriter::submit()
	{
		rhi::GpuDevice->writeDescriptorSet(descriptorSet, writeCommands.data(), writeCommands.size());
		writeCommands.clear();
	}

	RPIAttachmentSetCreator::RPIAttachmentSetCreator(RPIPipeline psoHandle)
	{
		creation.psoHandle = psoHandle; 
	}
	
	void RPIAttachmentSetCreator::setColorAttachment(int slot, RPIDescriptor descriptorHandle)
	{
		creation.attachments.push_back(rhi::RHIAttachmentBinding{ slot,false,descriptorHandle });
	}

	void RPIAttachmentSetCreator::setDepthStencilAttachment(RPIDescriptor descriptorHandle)
	{
		creation.attachments.push_back(rhi::RHIAttachmentBinding{ 0,true,descriptorHandle });
	}

	RPIAttachmentSet RPIAttachmentSetCreator::create()
	{
		return rhi::GpuDevice->createAttachmentSet(creation);
	}

	void RPIAttachmentSetDestoryer::destory()
	{
		rhi::GpuDevice->destoryAttachmentSet(handle);
	}
}