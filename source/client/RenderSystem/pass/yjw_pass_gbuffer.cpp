#include "yjw_pass_gbuffer.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include "../yjw_resource.h"

namespace yjw
{
    void GBufferPass::buildPSO()
    {
        vs = RPICreateShader(SHADER_FILE(gbuffer_vert.spv));
        ps = RPICreateShader(SHADER_FILE(gbuffer_frag.spv));

        RPIPipelineCreator pipelineCreator;
        pipelineCreator.addShaderEntry(RPIShaderType::vertex_shader, vs, "main");
        pipelineCreator.addShaderEntry(RPIShaderType::pixel_shader, ps, "main");
        pipelineCreator.addVertexAttribute(RPIFormat::R32G32B32_sfloat);
        pipelineCreator.addVertexAttribute(RPIFormat::R32G32B32_sfloat);
        pipelineCreator.addVertexAttribute(RPIFormat::R32G32_sfloat);
        pipelineCreator.addColorAttachment(RPIFormat::R8G8B8A8_snorm);
        pipelineCreator.addColorAttachment(RPIFormat::R8G8B8A8_snorm);
        pipelineCreator.addColorAttachment(RPIFormat::R32G32B32A32_sfloat);
        pipelineCreator.addColorAttachment(RPIFormat::R32G32B32A32_sfloat);
        pipelineCreator.addColorAttachment(RPIFormat::R32G32B32A32_sfloat);
        pipelineCreator.addDepthStencilAttachment(RPIFormat::D24_unorm_S8_uint);
        pipelineCreator.addDescriptor(RPIShaderType::vertex_shader, 0, 0, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 0, 1, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 0, 2, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 0, 3, RPIDescriptorType::uniform_buffer);
        pipelineCreator.addDescriptor(RPIShaderType::pixel_shader, 1, 0, RPIDescriptorType::shader_resource_texture);
        pipeline = pipelineCreator.create();
        descriptors_sets.resize(100);
        uniformsBuffers.resize(100);
        uniformsBufferDescriptors.resize(100);
        for (int i = 0; i < 100; i++)
        {
            descriptors_sets[i] = RPICreateDescriptorSet(pipeline);
            uniformsBuffers[i] = RPICreateUploadBuffer(48);
            uniformsBufferDescriptors[i] = RPICreateDescriptor(uniformsBuffers[i], RPIDescriptorType::uniform_buffer, RPIFormat::unknow);
        }
    }

    void GBufferPass::registerTexture(
        RPITexture out_abedlo,
        RPITexture out_normal,
        RPITexture out_diffuse,
        RPITexture out_specular,
        RPITexture out_ambient,
        RPITexture depth)
    {
        initialResourceState[out_abedlo] = RPIResourceState::render_target;
        initialResourceState[out_normal] = RPIResourceState::render_target;
        initialResourceState[out_diffuse] = RPIResourceState::render_target;
        initialResourceState[out_specular] = RPIResourceState::render_target;
        initialResourceState[out_ambient] = RPIResourceState::render_target;
        initialResourceState[depth] = RPIResourceState::depth_stencil_write;

        this->out_abeldo = RPICreateDescriptor(out_abedlo, RPIDescriptorType::render_target, RPIFormat::R8G8B8A8_snorm);
        this->out_normal = RPICreateDescriptor(out_abedlo, RPIDescriptorType::render_target, RPIFormat::R8G8B8A8_snorm);
        this->out_diffuse = RPICreateDescriptor(out_abedlo, RPIDescriptorType::render_target, RPIFormat::R32G32B32A32_sfloat);
        this->out_specular = RPICreateDescriptor(out_abedlo, RPIDescriptorType::render_target, RPIFormat::R32G32B32A32_sfloat);
        this->out_ambient = RPICreateDescriptor(out_abedlo, RPIDescriptorType::render_target, RPIFormat::R32G32B32A32_sfloat);
        this->out_depth = RPICreateDescriptor(out_abedlo, RPIDescriptorType::depth_stencil, RPIFormat::D24_unorm_S8_uint);

        RPIAttachmentSetCreator attachmentCreator(pipeline);
        attachmentCreator.setColorAttachment(0, this->out_abeldo);
        attachmentCreator.setColorAttachment(1, this->out_normal);
        attachmentCreator.setColorAttachment(2, this->out_diffuse);
        attachmentCreator.setColorAttachment(3, this->out_specular);
        attachmentCreator.setColorAttachment(4, this->out_ambient);
        attachmentCreator.setDepthStencilAttachment(this->out_depth);
        attachementSet = attachmentCreator.create();
    }

    void GBufferPass::setupData()
    {
        entitys = RenderSystem::get().scene.buildEntitys();
        for (int i = 0; i < entitys.size(); i++)
        {
            struct Material
            {
                alignas(16) glm::vec4 diffuse;
                alignas(16) glm::vec4 specular;
                alignas(16) glm::vec3 ambient;
            }material_data;
            material_data.diffuse = entitys[i].material->diffuse;
            material_data.specular = glm::vec4(entitys[i].material->specular, entitys[i].material->specularPower);
            material_data.ambient = entitys[i].material->ambient;

            RPIUpdateResource(uniformsBuffers[i], &material_data, 0, sizeof(Material));

            RPIDescriptorSetWriter writer(descriptors_sets[i]);
            writer.setDescriptor(0, 0, g_resource_store.cameraUniformDescriptor);
            writer.setDescriptor(0, 1, g_resource_store.lightUniformDescriptor);
            writer.setDescriptor(0, 2, g_resource_store.cameraUniformDescriptor);
            writer.setDescriptor(0, 3, uniformsBufferDescriptors[i]);
            writer.setDescriptor(1, 0, entitys[i].material->textureShaderResource);
            writer.submit();
        }
    }

    void GBufferPass::recordCommand(RPICommandBuffer commandBuffer)
    {
        RPIDrawer drawer(pipeline, commandBuffer, attachementSet);
        drawer.cmdBeginPass();
        for (int i = 0; i < entitys.size(); i++)
        {
            drawer.cmdBindDescriptorSet(descriptors_sets[i]);
            drawer.cmdBindVertexBuffer(entitys[i].mesh->vertex_buffer);
            drawer.cmdBindIndexBuffer(entitys[i].mesh->index_buffer);
            drawer.cmdDrawIndex(entitys[i].mesh->subMeshes[entitys[i].subMeshId].size, 1, entitys[i].mesh->subMeshes[entitys[i].subMeshId].offset, 0, 0);
        }
        drawer.cmdBeginPass();
    }

    void GBufferPass::submit()
    {

    }
}