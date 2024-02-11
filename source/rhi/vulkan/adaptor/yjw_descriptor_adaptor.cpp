#include "yjw_descriptor_adaptor.h"
#include "yjw_property_adaptor.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include <cassert>

namespace vulkan
{
    VulkanDescriptorCreationAdaptor::operator VulkanDescriptorCreation()
    {
        VulkanDescriptorCreation creation{};
        creation.resource_type = get_resource_type(payload.resource);
        creation.format = FormatAdaptor(payload.format);
        if (creation.resource_type == VulkanResourceType::buffer)
        {
            creation.handle.buffer = payload.resource;
        }
        else if (creation.resource_type == VulkanResourceType::texture)
        {
            creation.handle.texture = payload.resource;
        }
        creation.descriptor_type = DescriptorTypeAdaptor(payload.type);
        return creation;
    }

    VulkanAttachmentSetCreationAdaptor::operator VulkanAttachmentSetCreation()
    {
        VulkanAttachmentSetCreation creation{};
        creation.psoHandle = payload.psoHandle;

        VulkanPSO* pso = creation.psoHandle.Get();
        creation.descriptors.resize(pso->attachmentCount);
        for (const rhi::RHIAttachmentBinding& binding : payload.attachments)
        {
            if (binding.isDepth)
            {
                creation.descriptors.back() = binding.descriptor;
            }
            else
            {
                creation.descriptors[binding.slot] = binding.descriptor;
            }
        }

        return creation;
    }
}