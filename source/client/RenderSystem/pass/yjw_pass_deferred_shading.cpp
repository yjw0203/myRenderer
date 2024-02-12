#include "yjw_pass_deferred_shading.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include <memory>
#include "../yjw_resource.h"
namespace yjw
{
    void DeferredShadingPass::buildPSO()
    {
        glm::vec2 vertex[6] = { glm::vec2(-1,-1),glm::vec2(1,-1),glm::vec2(-1,1),glm::vec2(1,-1),glm::vec2(1,1),glm::vec2(-1,1) };

        vertex_buffer = RPICreateGpuVertexBuffer(sizeof(vertex));
        RPIUpdateResource(vertex_buffer, vertex, 0, sizeof(vertex));

        vs = RPICreateShader(SHADER_FILE(deferred_shading_vert.spv));
        ps = RPICreateShader(SHADER_FILE(deferred_shading_frag.spv));

        RPIPipelineCreator pipelineCreator;
        pipelineCreator.addShaderEntry(RPIShaderType::vertex_shader, vs, "main");
        pipelineCreator.addShaderEntry(RPIShaderType::pixel_shader, ps, "main");
        pipelineCreator.addVertexAttribute(RPIFormat::R32G32_sfloat);
        pipelineCreator.addColorAttachment(RPIFormat::R8G8B8A8_unorm);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 0, 0, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 0, 1, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 0, 2, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 0, RPIDescriptorType::shader_resource_texture);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 1, RPIDescriptorType::shader_resource_texture);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 2, RPIDescriptorType::shader_resource_texture);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 3, RPIDescriptorType::shader_resource_texture);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 4, RPIDescriptorType::shader_resource_texture);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 5, RPIDescriptorType::shader_resource_texture);
        pipeline = pipelineCreator.create();

        descriptors_set = RPICreateDescriptorSet(pipeline);
    }

    void DeferredShadingPass::registerTexture(
        RPITexture in_abeldo,
        RPITexture in_normal,
        RPITexture in_diffuse,
        RPITexture in_specular,
        RPITexture in_ambient,
        RPITexture in_depth,
        RPITexture out_color)
    {
        initialResourceState[in_abeldo] = RPIResourceState::shader_resource_read;
        initialResourceState[in_normal] = RPIResourceState::shader_resource_read;
        initialResourceState[in_diffuse] = RPIResourceState::shader_resource_read;
        initialResourceState[in_specular] = RPIResourceState::shader_resource_read;
        initialResourceState[in_ambient] = RPIResourceState::shader_resource_read;
        initialResourceState[in_depth] = RPIResourceState::shader_resource_read;
        initialResourceState[out_color] = RPIResourceState::render_target;

        this->in_abeldo = RPICreateDescriptor(in_abeldo, RPIDescriptorType::shader_resource_texture, RPIFormat::R8G8B8A8_snorm);
        this->in_normal = RPICreateDescriptor(in_normal, RPIDescriptorType::shader_resource_texture, RPIFormat::R8G8B8A8_snorm);
        this->in_diffuse = RPICreateDescriptor(in_diffuse, RPIDescriptorType::shader_resource_texture, RPIFormat::R32G32B32A32_sfloat);
        this->in_specular = RPICreateDescriptor(in_specular, RPIDescriptorType::shader_resource_texture, RPIFormat::R32G32B32A32_sfloat);
        this->in_ambient = RPICreateDescriptor(in_ambient, RPIDescriptorType::shader_resource_texture, RPIFormat::R32G32B32A32_sfloat);
        this->in_depth = RPICreateDescriptor(in_ambient, RPIDescriptorType::shader_resource_texture, RPIFormat::D24_unorm_S8_uint);
        this->out_color = RPICreateDescriptor(out_color, RPIDescriptorType::render_target, RPIFormat::R8G8B8A8_unorm);
        RPIAttachmentSetCreator attachmentSetCreator(pipeline);
        attachmentSetCreator.setColorAttachment(0, this->out_color);
        attachementSet = attachmentSetCreator.create();
    }

    void DeferredShadingPass::setupData()
    {
        RPIDescriptorSetWriter writer(this->descriptors_set);
        writer.setDescriptor(0, 0, g_resource_store.cameraUniformDescriptor);
        writer.setDescriptor(0, 1, g_resource_store.lightUniformDescriptor);
        writer.setDescriptor(0, 2, g_resource_store.optionUniformDescriptor);
        writer.setDescriptor(1, 0, in_abeldo);
        writer.setDescriptor(1, 1, in_normal);
        writer.setDescriptor(1, 2, in_diffuse);
        writer.setDescriptor(1, 3, in_specular);
        writer.setDescriptor(1, 4, in_ambient);
        writer.setDescriptor(1, 5, in_depth);
        writer.submit();
    }

    void DeferredShadingPass::recordCommand(RPICommandBuffer commandBuffer)
    {
        RPIDrawer drawer(pipeline, commandBuffer, attachementSet);
        drawer.cmdBeginPass();
        drawer.cmdBindDescriptorSet(descriptors_set);
        drawer.cmdBindVertexBuffer(vertex_buffer);
        drawer.cmdDraw(6, 1, 0, 0);
        drawer.cmdEndPass();
    }

    void DeferredShadingPass::submit()
    {

    }
}