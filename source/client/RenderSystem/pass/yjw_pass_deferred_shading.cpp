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

        vertex_buffer = std::make_unique<rhi::RHIBuffer>(sizeof(vertex), rhi::RHIResourceUsageBits::allow_vertex_buffer, rhi::RHIMemoryType::default_);
        rhi::IRHI::Get()->writeResourceImmidiately(vertex_buffer.get(), vertex, sizeof(vertex), 0);

        vs = std::make_shared<RHIShader>(SHADER_FILE(deferred_shading_vert.spv));
        ps = std::make_shared<RHIShader>(SHADER_FILE(deferred_shading_frag.spv));

        pipeline = RHIPipeline::NewGraphicsPipline()
            .VS(vs, "main")
            .PS(ps, "main")
            .RASTERIZATION_STATE(Rasterization_default)
            .COLOR_BLEND_STATE(ColorBlend_default)
            .DEPTH_STENCIL_STATE(DepthStencil_default)
            .VERTEX_BINDING<0, 0>(R32G32_sfloat)
            .COLOR_ATTACHMENT<0>(R8G8B8A8_unorm)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 0>(144)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 1>(32)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 2>(16)
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 0>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 1>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 2>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 3>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 4>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 5>();
    }

    void DeferredShadingPass::registerTexture(
        std::shared_ptr<RHIResource> in_abeldo,
        std::shared_ptr<RHIResource> in_normal,
        std::shared_ptr<RHIResource> in_diffuse,
        std::shared_ptr<RHIResource> in_specular,
        std::shared_ptr<RHIResource> in_ambient,
        std::shared_ptr<RHIResource> in_depth,
        std::shared_ptr<RHIResource> out_color)
    {
        initialResourceState[in_abeldo] = RHIResourceState::shader_resource_read;
        initialResourceState[in_normal] = RHIResourceState::shader_resource_read;
        initialResourceState[in_diffuse] = RHIResourceState::shader_resource_read;
        initialResourceState[in_specular] = RHIResourceState::shader_resource_read;
        initialResourceState[in_ambient] = RHIResourceState::shader_resource_read;
        initialResourceState[in_depth] = RHIResourceState::shader_resource_read;
        initialResourceState[out_color] = RHIResourceState::render_target;

        this->in_abeldo = SHADER_RESOURCE_TEXTURE(in_abeldo.get(), R8G8B8A8_snorm);
        this->in_normal = SHADER_RESOURCE_TEXTURE(in_normal.get(), R8G8B8A8_snorm);
        this->in_diffuse = SHADER_RESOURCE_TEXTURE(in_diffuse.get(), R32G32B32A32_sfloat);
        this->in_specular = SHADER_RESOURCE_TEXTURE(in_specular.get(), R32G32B32A32_sfloat);
        this->in_ambient = SHADER_RESOURCE_TEXTURE(in_ambient.get(), R32G32B32A32_sfloat);
        this->in_depth = SHADER_RESOURCE_TEXTURE(in_depth.get(), RHIFormat::D24_unorm_S8_uint);
        attachementSet = RHIAttachmentsSet::New(pipeline, 720, 720)
            .COLOR_ATTACHMENT<0>(COLOR_ATTACHMENT(out_color.get(), RHIFormat::R8G8B8A8_unorm));
    }

    void DeferredShadingPass::setupData()
    {
        descriptors_set = RHIDescriptorsSet::New(pipeline)
            .DESCRIPTOR<0, 0>(g_resource_store.cameraUniform)
            .DESCRIPTOR<0, 1>(g_resource_store.lightUniform)
            .DESCRIPTOR<0, 2>(g_resource_store.optionUniform)
            .DESCRIPTOR<1, 0>(in_abeldo)
            .DESCRIPTOR<1, 1>(in_normal)
            .DESCRIPTOR<1, 2>(in_diffuse)
            .DESCRIPTOR<1, 3>(in_specular)
            .DESCRIPTOR<1, 4>(in_ambient)
            .DESCRIPTOR<1, 5>(in_depth);
    }

    void DeferredShadingPass::recordCommand()
    {
        RHIExecutor executor(pipeline, attachementSet);
        executor.beginPass();
        executor.bindVertexBuffer(vertex_buffer.get());
        executor.bindDescriptorSet(descriptors_set.get());
        executor.draw(6, 1, 0, 0);
        executor.endPass();
    }

    void DeferredShadingPass::submit()
    {

    }
}