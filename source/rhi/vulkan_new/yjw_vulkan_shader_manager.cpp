#include "yjw_vulkan_shader_manager.h"
#include "yjw_vulkan_god_objects_manager.h"

#include <vector>
#include <string>
#include <fstream>

namespace rhi
{
    static std::vector<char> readFile(const char* filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    const VulkanShaderDesc& VulkanShaderLocation::getDesc()
    {
        return desc;
    }

    VkShaderModule VulkanShaderLocation::getShaderModule()
    {
        return shaderModule;
    }

    RHIShaderLocation* VulkanShaderManager::createShader(const RHIShaderDesc& rhi_desc)
    {
        VulkanShaderLocation* shader = new VulkanShaderLocation();
        VulkanShaderDesc& desc = shader->desc;
        desc.type = rhi_desc.type;

        auto code = readFile(rhi_desc.filePath);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(vulkanGod.device, &createInfo, nullptr, &shader->shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shader;
    }

    void VulkanShaderManager::deleteShader(RHIShaderLocation*& location)
    {
        VulkanShaderLocation* shader = (VulkanShaderLocation*)location;
        vkDestroyShaderModule(vulkanGod.device, shader->shaderModule, nullptr);
        delete location;
        location = nullptr;
    }
}