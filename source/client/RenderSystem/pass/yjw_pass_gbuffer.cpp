#include "yjw_pass_gbuffer.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include "../yjw_resource.h"

namespace yjw
{
    void GBufferPass::buildPSO()
    {
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
            .COLOR_ATTACHMENT<2>(R32G32B32A32_sfloat)
            .COLOR_ATTACHMENT<3>(R32G32B32A32_sfloat)
            .COLOR_ATTACHMENT<4>(R32G32B32A32_sfloat)
            .DEETH_STENCIL_ATTACHMENT(D24_unorm_S8_uint)
            .UNIFORM_BUFFER<RHIShaderType::vertex_shader, 0, 0>(144)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 1>(32)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 2>(144)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 3>(48)
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 0>();
    }

    void GBufferPass::registerTexture(
        std::shared_ptr<RHIResource> out_abedlo,
        std::shared_ptr<RHIResource> out_normal,
        std::shared_ptr<RHIResource> out_diffuse,
        std::shared_ptr<RHIResource> out_specular,
        std::shared_ptr<RHIResource> out_ambient,
        std::shared_ptr<RHIResource> depth)
    {
        initialResourceState[out_abedlo] = RHIResourceState::render_target;
        initialResourceState[out_normal] = RHIResourceState::render_target;
        initialResourceState[out_diffuse] = RHIResourceState::render_target;
        initialResourceState[out_specular] = RHIResourceState::render_target;
        initialResourceState[out_ambient] = RHIResourceState::render_target;
        initialResourceState[depth] = RHIResourceState::depth_stencil_write;

        attachementSet = RHIAttachmentsSet::New(pipeline, 720, 720)
            .COLOR_ATTACHMENT<0>(COLOR_ATTACHMENT(out_abedlo.get(), RHIFormat::R8G8B8A8_snorm))
            .COLOR_ATTACHMENT<1>(COLOR_ATTACHMENT(out_normal.get(), RHIFormat::R8G8B8A8_snorm))
            .COLOR_ATTACHMENT<2>(COLOR_ATTACHMENT(out_diffuse.get(), RHIFormat::R32G32B32A32_sfloat))
            .COLOR_ATTACHMENT<3>(COLOR_ATTACHMENT(out_specular.get(), RHIFormat::R32G32B32A32_sfloat))
            .COLOR_ATTACHMENT<4>(COLOR_ATTACHMENT(out_ambient.get(), RHIFormat::R32G32B32A32_sfloat))
            .DEPTH_STENCIL_ATTACHMENT(DEPTH_STENCIL_ATTACHMENT(depth.get(), RHIFormat::D24_unorm_S8_uint));
    }

    void GBufferPass::setupData()
    {
        entitys = RenderSystem::get().scene.buildEntitys();
        descriptors_sets.resize(entitys.size());
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

            std::shared_ptr<RHIUniformBuffer> material = std::make_shared<RHIUniformBuffer>(48);
            material->update(&material_data, sizeof(Material), 0);

            descriptors_sets[i] = RHIDescriptorsSet::New(pipeline)
                .DESCRIPTOR<0, 0>(g_resource_store.cameraUniform)
                .DESCRIPTOR<0, 1>(g_resource_store.lightUniform)
                .DESCRIPTOR<0, 2>(g_resource_store.cameraUniform)
                .DESCRIPTOR<0, 3>(material)
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