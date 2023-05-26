#include "yjw_pass_gbuffer.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"

namespace yjw
{
    void GBufferPass::buildPSO()
    {
        camera_uniform = std::make_shared<RHIUniformBuffer>(128);
        light_uniform = std::make_shared<RHIUniformBuffer>(32);
        camera_pos_uniform = std::make_shared<RHIUniformBuffer>(16);

        vs = std::make_shared<RHIShader>(SHADER_FILE(gbuffer_vert.spv));
        ps = std::make_shared<RHIShader>(SHADER_FILE(gbuffer_frag.spv));

        pipeline = RHIPipeline::NewGraphicsPipline()
            .VS(vs, "main")
            .PS(ps, "main")
            .RASTERIZATION_STATE(Rasterization_default)
            .COLOR_BLEND_STATE(ColorBlend_default)
            .DEPTH_STENCIL_STATE(DepthStencil_default)
            .VERTEX_BINDING<0, 0>(R32G32B32_sfloat)
            .VERTEX_BINDING<0, 1>(R32G32B32_sfloat)
            .VERTEX_BINDING<0, 2>(R32G32_sfloat)
            .COLOR_ATTACHMENT<0>(R8G8B8A8_snorm)
            .COLOR_ATTACHMENT<1>(R8G8B8A8_snorm)
            .DEETH_STENCIL_ATTACHMENT(D24_unorm_S8_uint)
            .UNIFORM_BUFFER<RHIShaderType::vertex_shader, 0, 0>(128)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 1>(32)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 2>(16)
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 0>();
    }

    void GBufferPass::registerTexture(std::shared_ptr<RHIColorAttachment> out_abedlo, std::shared_ptr<RHIColorAttachment> out_normal, std::shared_ptr<RHIDepthStencilAttachment> depth)
    {
        attachementSet = RHIAttachmentsSet::New(pipeline, 720, 720)
            .COLOR_ATTACHMENT<0>(out_abedlo)
            .COLOR_ATTACHMENT<1>(out_normal)
            .DEPTH_STENCIL_ATTACHMENT(depth);
    }

    void GBufferPass::setupData()
    {
        RenderCamera& camera = RenderSystem::get().activeCamera;
        struct Data
        {
            glm::mat4x4 view;
            glm::mat4x4 project;
        }data;

        data.view = camera.getViewMatrix();
        data.project = camera.getProjectionMatrix();
        camera_uniform->update(&data, sizeof(data), 0);
        camera_pos_uniform->update(&camera.position, sizeof(camera.position), 0);

        struct Light
        {
            alignas(16) glm::vec3 pos;
            alignas(16) glm::vec3 color;
        };
        Light light;
        light.pos = glm::vec3(-3, 15, -8);
        light.color = glm::vec3(1, 1, 1);
        light_uniform->update(&light, sizeof(light), 0);

        entitys = RenderSystem::get().scene.buildEntitys();

        descriptors_sets.resize(entitys.size());
        for (int i = 0; i < entitys.size(); i++)
        {
            descriptors_sets[i] = RHIDescriptorsSet::New(pipeline)
                .DESCRIPTOR<0, 0>(camera_uniform)
                .DESCRIPTOR<0, 1>(light_uniform)
                .DESCRIPTOR<0, 2>(camera_pos_uniform)
                .DESCRIPTOR<1, 0>(entitys[i].material->textureShaderResource);
        }
    }

    void GBufferPass::recordCommand()
    {
        RHIExecutor executor(pipeline, attachementSet);
        executor.beginPass();
        for (int i = 0; i < entitys.size(); i++)
        {
            executor.bindDescriptorSet(descriptors_sets[i].get());
            executor.bindVertexBuffer(entitys[i].mesh->vertex_buffer.get());
            executor.bindIndexBuffer(entitys[i].mesh->index_buffer.get());
            executor.drawIndex(entitys[i].mesh->subMeshes[entitys[i].subMeshId].size, 1, entitys[i].mesh->subMeshes[entitys[i].subMeshId].offset, 0, 0);
        }
        executor.endPass();
    }

    void GBufferPass::submit()
    {

    }
}