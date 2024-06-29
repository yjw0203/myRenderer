#include "Private/vulkan/yjw_vulkan_buffer.h"
#include "Private/vulkan/yjw_vulkan_type_conversation.h"
#include "Private/vulkan/yjw_vulkan_common.h"
#include "Private/vulkan/yjw_vulkan_command_list.h"

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

    void VulkanBuffer::Update(void* data, int bufferOffset, int sizeOfByte)
    {
        if (GetDesc().memoryType == RHIMemoryType::default_)
        {
            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;
            vkCreateBuffer(GetDevice()->GetNativeDevice(), GetDevice()->GetGpu(), sizeOfByte, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

            void* map_data;
            vkMapMemory(GetDevice()->GetNativeDevice(), stagingBufferMemory, 0, sizeOfByte, 0, &map_data);
            memcpy(map_data, data, (size_t)sizeOfByte);
            vkUnmapMemory(GetDevice()->GetNativeDevice(), stagingBufferMemory);

            VkBufferCopy copyRegion{};
            copyRegion.dstOffset = bufferOffset;
            copyRegion.size = sizeOfByte;
            vkCmdCopyBuffer(GetDevice()->GetImmediaCommandList()->GetCommandBuffer(), stagingBuffer, m_buffer, 1, &copyRegion);

            GetDevice()->GetImmediaCommandList()->Submit();

            GetDevice()->WaitForIdle();

            vkDestroyBuffer(GetDevice()->GetNativeDevice(), stagingBuffer, nullptr);
            vkFreeMemory(GetDevice()->GetNativeDevice(), stagingBufferMemory, nullptr);
        }
        else
        {
            void* buffer_map;
            vkMapMemory(GetDevice()->GetNativeDevice(), m_memory, bufferOffset, sizeOfByte, 0, &buffer_map);
            memcpy(buffer_map, data, static_cast<size_t>(sizeOfByte));
            vkUnmapMemory(GetDevice()->GetNativeDevice(), m_memory);
        }
    }

    VulkanBufferView::VulkanBufferView(VulkanDevice* device, const RHIBufferViewDescriptor& desc)
        : VulkanDeviceObject(device)
        , RHIBufferView(desc)
    {
        /*
        VkBufferViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
        createInfo.buffer = GetBuffer()->GetVkBuffer();
        createInfo.format = VK_FORMAT_UNDEFINED;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.offset = desc.offset;
        createInfo.range = desc.offset + desc.width;
        
        vkCreateBufferView(device->GetNativeDevice(), &createInfo, nullptr, &m_view);*/
        desc.buffer->retain(this);
    }

    VulkanBufferView::~VulkanBufferView()
    {
        //vkDestroyBufferView(GetDevice()->GetNativeDevice(), m_view, nullptr);
        GetDesc().buffer->release();
    }

    VulkanBuffer* VulkanBufferView::GetBuffer()
    {
        return VKResourceCast(GetDesc().buffer);
    }

    int VulkanBufferView::GetOffset()
    {
        return GetDesc().offset;
    }
}