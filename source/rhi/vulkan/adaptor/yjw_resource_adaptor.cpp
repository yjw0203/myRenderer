#include "yjw_resource_adaptor.h"
#include "yjw_property_adaptor.h"
#include <cassert>

namespace vulkan
{
    ResourceCreationAdaptor::operator VulkanTextureCreation()
    {
        assert(payload.type != rhi::ResourceType::buffer);
        assert(payload.type != rhi::ResourceType::unkown);

        VulkanTextureCreation initConfig;
        initConfig.width = payload.width;
        initConfig.height = payload.height;
        initConfig.mipLevels = payload.miplevels;
        switch (payload.type)
        {
        case rhi::ResourceType::texture1D: {
            initConfig.type = VkImageType::VK_IMAGE_TYPE_1D;
            initConfig.depth = 1;
            initConfig.arrayLayers = 1;
        } break;
        case rhi::ResourceType::texture2D: {
            initConfig.type = VkImageType::VK_IMAGE_TYPE_2D;
            initConfig.depth = 1;
            initConfig.arrayLayers = 1;
        }break;
        case rhi::ResourceType::texture3D: {
            initConfig.type = VkImageType::VK_IMAGE_TYPE_3D;
            initConfig.depth = payload.depthOrArraySize;
            initConfig.arrayLayers = 1;
        }break;
        case rhi::ResourceType::textureCube: {
            initConfig.type = VkImageType::VK_IMAGE_TYPE_2D;
            initConfig.depth = 1;
            initConfig.arrayLayers = 6;
        }break;
        case rhi::ResourceType::texture2DArray: {
            initConfig.type = VkImageType::VK_IMAGE_TYPE_2D;
            initConfig.depth = 1;
            initConfig.arrayLayers = payload.depthOrArraySize;

        }break;
        }
        initConfig.format = FormatAdaptor(payload.format);
        initConfig.usage = ImageUsageFlagsAdptor(payload.usage);
        initConfig.memoryType = MemoryPropertyFlagsAdptor(payload.memoryType);
        return std::move(initConfig);
    }

    ResourceCreationAdaptor::operator VulkanBufferCreation()
    {
        assert(payload.type == rhi::ResourceType::buffer);
        VulkanBufferCreation initConfig;
        initConfig.size = payload.width;
        initConfig.usage = ImageUsageFlagsAdptor(payload.usage);
        initConfig.memoryType = MemoryPropertyFlagsAdptor(payload.memoryType);
        return std::move(initConfig);
    }
}