#include "yjw_default_pipeline.h"
#include "client/RenderSystem/pass/yjw_pass_gbuffer.h"
#include "client/RenderSystem/pass/yjw_pass_deferred_shading.h"

namespace yjw
{
    void DefaultPipeline::initializeResource()
    {
        RPITexture albedo_image = RPICreateDefaultTexture2D(720, 720, RPIFormat::R8G8B8A8_snorm);
        RPITexture normal_image = RPICreateDefaultTexture2D(720, 720, RPIFormat::R8G8B8A8_snorm);
        RPITexture depthImage = RPICreateDepthStencilTexture2D(720, 720, RPIFormat::D24_unorm_S8_uint);
        RPITexture colorImage = RPICreateDefaultTexture2D(720, 720, RPIFormat::R8G8B8A8_unorm);
        RPITexture diffuseImage = RPICreateDefaultTexture2D(720, 720, RPIFormat::R32G32B32A32_sfloat);
        RPITexture specularImage = RPICreateDefaultTexture2D(720, 720, RPIFormat::R32G32B32A32_sfloat);
        RPITexture ambientImage = RPICreateDefaultTexture2D(720, 720, RPIFormat::R32G32B32A32_sfloat);
        texture_map["albedo"] = Resource{ albedo_image ,true };
        texture_map["normal"] = Resource{ normal_image ,true };
        texture_map["depth"] = Resource{ depthImage ,true };
        texture_map["color"] = Resource{ colorImage ,true };
        texture_map["diffuse"] = Resource{ diffuseImage ,true };
        texture_map["specular"] = Resource{ specularImage ,true };
        texture_map["ambient"] = Resource{ ambientImage ,true };

        output = colorImage;

        commandBuffer = RPICreateCommandBuffer();
    }

    void DefaultPipeline::config()
    {
        std::shared_ptr<GBufferPass> gbuffer_pass = std::make_shared<GBufferPass>();
        std::shared_ptr<DeferredShadingPass> deferred_shading_pass = std::make_shared<DeferredShadingPass>();

        deferred_shading_pass->buildPSO();
        gbuffer_pass->buildPSO();

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

        passes.push_back(gbuffer_pass);
        passes.push_back(deferred_shading_pass);
    }

    void DefaultPipeline::render()
    {
        RPIResetCommandBuffer(commandBuffer);
        for (auto pass : passes)
        {
            pass->setupData();
        }

        for (auto& texture : texture_map)
        {
            //IRHI::Get()->resourceBarrier(texture.second.resource_handle.get(), texture.second.resource_handle->state, RHIResourceState::transfer_dst);
            ///IRHI::Get()->clearImageResource(texture.second.resource_handle.get());
        }

        for (auto pass : passes)
        {
            pass->setResourceBarrier();
            pass->recordCommand(commandBuffer);
        }
    }
}