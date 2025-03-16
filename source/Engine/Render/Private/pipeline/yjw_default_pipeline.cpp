#include "yjw_default_pipeline.h"
#include "Engine/Render/Private/pass1/yjw_pass_draw_triangle.h"
#include "Engine/Render/Private/pass1/yjw_pass_gbuffer.h"
#include "Engine/Render/Private/pass1/yjw_pass_apply_to_window.h"
#include "Engine/Render/Private/pass1/yjw_pass_deferred_shading.h"
#include "Engine/Render/Private/pass1/yjw_pass_imgui.h"
#include "Engine/Render/Private/pass1/yjw_pass_forward.h"
#include "Engine/Render/Private/pass1/yjw_pass_tone_mapping.h"


namespace yjw
{
    void DefaultPipeline::initializeResource()
    {
        RPITexture albedo_image = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_snorm);
        RPITexture normal_image = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_snorm);
        RPITexture depthImage = RPICreateDepthStencilTexture2D(1200, 1200, RPIFormat::D24_unorm_S8_uint);
        RPITexture shadingResultImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_unorm);
        RPITexture colorImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_unorm);
        RPITexture outputImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_unorm);
        RPITexture diffuseImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
        RPITexture specularImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
        RPITexture ambientImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
        texture_map["albedo"] = Resource{ albedo_image ,true };
        texture_map["normal"] = Resource{ normal_image ,true };
        texture_map["depth"] = Resource{ depthImage ,true };
        texture_map["shadingResult"] = Resource{ shadingResultImage ,true };
        texture_map["color"] = Resource{ colorImage ,true };
        texture_map["output"] = Resource{ outputImage ,true };
        texture_map["diffuse"] = Resource{ diffuseImage ,true };
        texture_map["specular"] = Resource{ specularImage ,true };
        texture_map["ambient"] = Resource{ ambientImage ,true };

        output = outputImage;
        m_scene_texture = colorImage;

        commandBuffer = RPICreateContext();
    }

    void DefaultPipeline::config(DefaultPipelineConfig config)
    {
        //std::shared_ptr<GBufferPass> gbuffer_pass = std::make_shared<GBufferPass>();
        //std::shared_ptr<DeferredShadingPass> deferred_shading_pass = std::make_shared<DeferredShadingPass>();
        forwardPass = std::make_shared<ForwardPass>();
        std::shared_ptr<ApplyToWindowPass> applyToWindowPass = std::make_shared<ApplyToWindowPass>(config.window);
        std::shared_ptr<ToneMappingPass> toneMappingPass = std::make_shared<ToneMappingPass>();
        debugPass = std::make_shared<DebugPass>();
        std::shared_ptr<DrawImGuiPass> drawImGuiPass = std::make_shared<DrawImGuiPass>();

        drawImGuiPass->setData(m_ui);

        //passes.push_back(gbuffer_pass);
        //passes.push_back(deferred_shading_pass);
        passes.push_back(forwardPass);
        passes.push_back(toneMappingPass);
        passes.push_back(drawImGuiPass);
        passes.push_back(debugPass);
        passes.push_back(applyToWindowPass);

        for (auto pass : passes)
        {
            pass->buildPSO();
        }
        forwardPass->registerTexture(texture_map["shadingResult"].resource_handle, texture_map["depth"].resource_handle);
        toneMappingPass->registerTexture(texture_map["shadingResult"].resource_handle, texture_map["color"].resource_handle);
        drawImGuiPass->registerTexture(texture_map["output"].resource_handle);
        debugPass->registerTexture(texture_map["shadingResult"].resource_handle);
        applyToWindowPass->registerTexture(texture_map["shadingResult"].resource_handle);
    }

    void DefaultPipeline::render()
    {
        forwardPass->attachScene(m_scene);
        forwardPass->setData();
        for (auto pass : passes)
        {
            pass->setupData();
        }

        for (auto& texture : texture_map)
        {
            RPICmdClearTexture(commandBuffer, texture.second.resource_handle);
        }

        for (auto pass : passes)
        {
            pass->recordCommand(commandBuffer);
        }
    }

    void DefaultPipeline::ClearDebugLine()
    {
        if (debugPass)
        {
            debugPass->Clear();
        }
    }

    void DefaultPipeline::AddDebugLine(glm::vec3 point0, glm::vec3 point1)
    {
        if (debugPass)
        {
            debugPass->AddLine(point0, point1);
        }
    }

    void DefaultPipeline::AttachScene(Scene* scene)
    {
        m_scene = scene;
    }

    void DefaultPipeline::AttachUI(rhi::ImGuiUI* ui)
    {
        m_ui = ui;
    }
}