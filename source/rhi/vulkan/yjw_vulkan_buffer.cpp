#include "yjw_vulkan_buffer.h"
#include "yjw_vulkan_type_conversation.h"

namespace rhi
{
    uint32_t findMemoryType_(VkPhysicalDevice gpu, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0;
    }

    VulkanBuffer::VulkanBuffer(VulkanDevice* device, const RHIBufferDescriptor& desc)
        :VulkanDeviceObject(device)
        ,RHIBuffer(desc)
    {
        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.pNext = nullptr;
        createInfo.usage = ConvertBufferUsageToVkBufferUsage(desc.usage);
        createInfo.size = desc.width;
        createInfo.flags = 0;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;

        vkCreateBuffer(device->GetNativeDevice(), &createInfo, nullptr, &m_buffer);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device->GetNativeDevice(), m_buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType_(device->GetGpu(),memRequirements.memoryTypeBits, ConvertMemoryTypeToVkMemoryPropertyFlags(desc.memoryType));

        vkAllocateMemory(device->GetNativeDevice(), &allocInfo, nullptr, &m_memory);

        vkBindBufferMemory(device->GetNativeDevice(), m_buffer, m_memory, 0);
    }

    VulkanBuffer::~VulkanBuffer()
    {
        vkFreeMemory(GetDevice()->GetNativeDevice(), m_memory, nullptr);
        vkDestroyBuffer(GetDevice()->GetNativeDevice(), m_buffer, nullptr);
    }

    VkBuffer VulkanBuffer::GetVkBuffer()
    {
        return m_buffer;
    }

    VkDeviceMemory VulkanBuffer::GetVkDeviceMemory()
    {
        return m_memory;
    }

    VulkanBufferView::VulkanBufferView(VulkanDevice* device, const RHIBufferViewDescriptor& desc)
        : VulkanDeviceObject(device)
        , RHIBufferView(desc)
    {
        VkBufferViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        createInfo.buffer = GetBuffer()->GetVkBuffer();
        createInfo.format = VK_FORMAT_UNDEFINED;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.offset = desc.offset;
        createInfo.range = desc.offset + desc.width;
        
        vkCreateBufferView(device->GetNativeDevice(), &createInfo, nullptr, &m_view);
        desc.buffer->retain(this);
    }

    VulkanBufferView::~VulkanBufferView()
    {
        vkDestroyBufferView(GetDevice()->GetNativeDevice(), m_view, nullptr);
        GetDesc().buffer->release();
    }

    VulkanBuffer* VulkanBufferView::GetBuffer()
    {
        return ResourceCast(GetDesc().buffer);
    }
}