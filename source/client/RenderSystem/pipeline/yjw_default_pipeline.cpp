#include "yjw_default_pipeline.h"
#include "client/RenderSystem/pass/yjw_pass_draw_triangle.h"
#include "client/RenderSystem/pass/yjw_pass_gbuffer.h"
#include "client/RenderSystem/pass/yjw_pass_apply_to_window.h"
#include "client/RenderSystem/pass/yjw_pass_deferred_shading.h"

namespace yjw
{
    void DefaultPipeline::initializeResource()
    {
        RPITexture albedo_image = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_snorm);
        RPITexture normal_image = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_snorm);
        RPITexture depthImage = RPICreateDepthStencilTexture2D(1200, 1200, RPIFormat::D24_unorm_S8_uint);
        RPITexture colorImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R8G8B8A8_unorm);
        RPITexture diffuseImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
        RPITexture specularImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
        RPITexture ambientImage = RPICreateDefaultTexture2D(1200, 1200, RPIFormat::R32G32B32A32_sfloat);
        texture_map["albedo"] = Resource{ albedo_image ,true };
        texture_map["normal"] = Resource{ normal_image ,true };
        texture_map["depth"] = Resource{ depthImage ,true };
        texture_map["color"] = Resource{ colorImage ,true };
        texture_map["diffuse"] = Resource{ diffuseImage ,true };
        texture_map["specular"] = Resource{ specularImage ,true };
        texture_map["ambient"] = Resource{ ambientImage ,true };

        output = colorImage;

        commandBuffer = RPICreateContext();
    }

    void DefaultPipeline::config(DefaultPipelineConfig config)
    {
        std::shared_ptr<GBufferPass> gbuffer_pass = std::make_shared<GBufferPass>();
        std::shared_ptr<DeferredShadingPass> deferred_shading_pass = std::make_shared<DeferredShadingPass>();
        std::shared_ptr<ApplyToWindowPass> applyToWindowPass = std::make_shared<ApplyToWindowPass>(config.window);

        passes.push_back(gbuffer_pass);
        passes.push_back(deferred_shading_pass);
        passes.push_back(applyToWindowPass);

        for (auto pass : passes)
        {
            pass->buildPSO();
        }
        
        gbuffer_pass->registerTexture(
            texture_map["albedo"].resource_handle,
            texture_map["normal"].resource_handle,
            texture_map["diffuse"].resource_handle,
            texture_map["specular"].resource_handle,
            texture_map["ambient"].resource_handle,
            texture_map["depth"].resource_handle);
        deferred_shading_pass->registerTexture(
            texture_map["albedo"].resource_handle,
            texture_map["normal"].resource_handle,
            texture_map["diffuse"].resource_handle,
            texture_map["specular"].resource_handle,
            texture_map["ambient"].resource_handle,
            texture_map["depth"].resource_handle,
            texture_map["color"].resource_handle);
        applyToWindowPass->registerTexture(texture_map["color"].resource_handle);

    }

    void DefaultPipeline::render()
    {
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
}