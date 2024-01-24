#include "yjw_vulkan.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
#include "rhi/vulkan/shader/yjw_shader_header.h"
#include "rhi/vulkan/adaptor/yjw_resource_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_shader_adaptor.h"
#include "rhi/vulkan/adaptor/yjw_pso_adaptor.h"
#include "command/yjw_vulkan_command_buffer.h"

namespace vulkan
{
    void VulkanRHI::createResource(const rhi::ResourceInitConfig& initConfig, rhi::Resource*& resource)
    {
        if (initConfig.type == rhi::ResourceType::buffer)
        {
            Buffer* buffer = nullptr;
            VK_G(BufferPool).allocateBuffer(ResourceInitConfigAdaptor(initConfig), buffer);
            resource = new rhi::Resource(initConfig, buffer);
        }
        else
        {
            Texture* texture = nullptr;
            VK_G(TexturePool).createTexture(ResourceInitConfigAdaptor(initConfig), texture);
            resource = new rhi::Resource(initConfig, texture);
        }
    }

    void VulkanRHI::destoryResource(rhi::Resource*& resource)
    {
        if (resource->initConfig.type == rhi::ResourceType::buffer)
        {
            Buffer& buffer = BufferAdaptor(*resource);
            VK_G(BufferPool).deallocateBuffer(&buffer);
            delete resource;
        }
        else
        {
            Texture& texture = TextureAdaptor(*resource);
            VK_G(TexturePool).destroyTexture(&texture);
            delete resource;
        }
    }

    void VulkanRHI::createShader(const rhi::ShaderInitConfig& initConfig, rhi::Shader*& shader)
    {
        Shader* vk_shader = nullptr;
        VK_G(ShaderPool).createShaderFromFile(initConfig.name.c_str(), vk_shader);
        shader = new rhi::Shader(initConfig, vk_shader);
    }

    void VulkanRHI::destoryShader(rhi::Shader*& shader)
    {
        Shader* vk_shader = ShaderAdaptor(*shader);
        VK_G(ShaderPool).destoryShader(vk_shader);
        delete shader;
    }

    void VulkanRHI::createPSO(const rhi::PSOInitConfig& initConfig, rhi::PSO*& pso)
    {
        PSOFactoryAdaptor frac(initConfig);
        PSO* vk_pso = ((PSOFactory)frac).createGraphicsPSO();
        pso = new rhi::PSO();
        pso->setPayload(vk_pso);
    }

    void VulkanRHI::destoryPSO(rhi::PSO*& pso)
    {

    }

    void VulkanRHI::createCommandBuffer(const rhi::CommandBufferInitConfig& initConfig, rhi::CommandBuffer*& commandBuffer)
    {
        CommandBuffer* vk_command_buffer = nullptr;
        VK_G(CommandBufferPool).allocateCommandBuffer(vk_command_buffer);
        commandBuffer = new rhi::CommandBuffer();
        commandBuffer->setPayload(vk_command_buffer);
    }

    void VulkanRHI::destoryCommandBuffer(rhi::CommandBuffer*& commandBuffer)
    {
        CommandBuffer* vk_command_buffer = (CommandBuffer*)commandBuffer->getPayload();
        VK_G(CommandBufferPool).destoryCommandBuffer(vk_command_buffer);
        delete commandBuffer;
    }
}