#include "yjw_pass_deferred_shading.h"
#include "client/RenderSystem/yjw_render_system.h"
#include "generate/projectInfo.h"
#include <memory>
namespace yjw
{
    void DeferredShadingPass::buildPSO()
    {
        camera_uniform = std::make_shared<RHIUniformBuffer>(128);
        light_uniform = std::make_shared<RHIUniformBuffer>(32);
        camera_pos_uniform = std::make_shared<RHIUniformBuffer>(16);
        screen_uniform = std::make_shared<RHIUniformBuffer>(16);

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
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 0>(128)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 1>(32)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 2>(16)
            .UNIFORM_BUFFER<RHIShaderType::pixel_shader, 0, 3>(16)
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 0>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 1>()
            .TEXTURE_SRV<RHIShaderType::pixel_shader, 1, 2>();
    }

    void DeferredShadingPass::registerTexture(std::shared_ptr<RHIShaderResourceTexture> in_abeldo, std::shared_ptr<RHIShaderResourceTexture> in_normal, std::shared_ptr<RHIShaderResourceTexture> in_depth, std::shared_ptr<RHIColorAttachment> out_color)
    {
        this->in_abeldo = in_abeldo;
        this->in_normal = in_normal;
        this->in_depth = in_depth;
        attachementSet = RHIAttachmentsSet::New(pipeline, 720, 720)
            .COLOR_ATTACHMENT<0>(out_color);
    }

    void DeferredShadingPass::setupData()
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

        glm::vec2 screenSize = glm::vec2(720, 720);
        screen_uniform->update(&screenSize, sizeof(screenSize), 0);

        descriptors_set = RHIDescriptorsSet::New(pipeline)
            .DESCRIPTOR<0, 0>(camera_uniform)
            .DESCRIPTOR<0, 1>(light_uniform)
            .DESCRIPTOR<0, 2>(camera_pos_uniform)
            .DESCRIPTOR<0, 3>(screen_uniform)
            .DESCRIPTOR<1, 0>(in_abeldo)
            .DESCRIPTOR<1, 1>(in_normal)
            .DESCRIPTOR<1, 2>(in_depth);
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