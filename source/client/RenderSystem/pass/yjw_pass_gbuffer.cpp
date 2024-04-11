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

        RPIRenderPipelineDescriptor pipelineDesc = RPIGetDefaultRenderPipeline();
        pipelineDesc.vs = vs;
        pipelineDesc.vs_entry = "main";
        pipelineDesc.ps = ps;
        pipelineDesc.ps_entry = "main";
        pipelineDesc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_depth_read_and_write);
        pipeline = RPICreateRenderPipeline(pipelineDesc);

        resource_bindings.resize(100);
        uniformsBuffers.resize(100);
        uniformsBufferDescriptors.resize(100);
        for (int i = 0; i < 100; i++)
        {
            resource_bindings[i] = RPICreateResourceBinding(pipeline);
            uniformsBuffers[i] = RPICreateUploadBuffer(48);
            uniformsBufferDescriptors[i] = RPICreateBufferView(uniformsBuffers[i], 0, 48);
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
        this->out_abeldo = RPICreateTextureView(out_abedlo, RPIFormat::R8G8B8A8_snorm);
        this->out_normal = RPICreateTextureView(out_normal, RPIFormat::R8G8B8A8_snorm);
        this->out_diffuse = RPICreateTextureView(out_diffuse, RPIFormat::R32G32B32A32_sfloat);
        this->out_specular = RPICreateTextureView(out_specular,  RPIFormat::R32G32B32A32_sfloat);
        this->out_ambient = RPICreateTextureView(out_ambient, RPIFormat::R32G32B32A32_sfloat);
        this->out_depth = RPICreateTextureView(depth, RPIFormat::D24_unorm_S8_uint);

        RPITextureView texture[5] = { this->out_abeldo,this->out_normal ,this->out_diffuse ,this->out_specular ,this->out_ambient };
        renderPass = RPICreateRenderPass(texture, 5, this->out_depth);

        entitys = RenderSystem::get().scene.buildEntitys();
        for (int i = 0; i < entitys.size(); i++)
        {
            resource_bindings[i]->SetBufferView(RHIShaderType::vertex, RHIName("camera"), g_resource_store.cameraUniformDescriptor);
            resource_bindings[i]->SetBufferView(RHIShaderType::fragment, RHIName("light"), g_resource_store.lightUniformDescriptor);
            resource_bindings[i]->SetBufferView(RHIShaderType::fragment, RHIName("camerapos"), g_resource_store.cameraUniformDescriptor);
            resource_bindings[i]->SetBufferView(RHIShaderType::fragment, RHIName("material"), uniformsBufferDescriptors[i]);
            resource_bindings[i]->SetTextureView(RHIShaderType::fragment, RHIName("albedoTex"), entitys[i].material->textureShaderResource);
            resource_bindings[i]->SetVertexBuffer(RHIName("in_pos"), entitys[i].mesh->vertex_buffer.pos);
            resource_bindings[i]->SetVertexBuffer(RHIName("in_normal"), entitys[i].mesh->vertex_buffer.normal);
            resource_bindings[i]->SetVertexBuffer(RHIName("in_uv"), entitys[i].mesh->vertex_buffer.uv);
            resource_bindings[i]->SetIndexBuffer(entitys[i].mesh->index_buffer);
        }
    }

    void GBufferPass::setupData()
    {
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

            RPIUpdateBuffer(uniformsBuffers[i], &material_data, 0, sizeof(Material));
        }
    }

    void GBufferPass::recordCommand(RPIContext commandBuffer)
    {
        RPICmdBeginRenderPass(commandBuffer, renderPass, resource_bindings.data(), entitys.size());
        for (int i = 0; i < entitys.size(); i++)
        {
            RPICmdSetResourceBinding(commandBuffer, resource_bindings[i]);
            RPICmdSetPipeline(commandBuffer, pipeline);
            RPICmdDrawIndex(commandBuffer, entitys[i].mesh->subMeshes[entitys[i].subMeshId].size, 1, entitys[i].mesh->subMeshes[entitys[i].subMeshId].offset, 0, 0);
        }
        RPICmdEndPass(commandBuffer);
    }

    void GBufferPass::submit()
    {

    }
}