#include "yjw_default_pipeline.h"
#include "client/RenderSystem/pass/yjw_pass_gbuffer.h"
#include "client/RenderSystem/pass/yjw_pass_deferred_shading.h"

namespace yjw
{
    void DefaultPipeline::initializeResource()
    {
        std::shared_ptr <RHITexture2D> albedo_image = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::R8G8B8A8_snorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        std::shared_ptr <RHITexture2D> normal_image = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::R8G8B8A8_snorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        std::shared_ptr <RHITexture2D> depthImage = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::D24_unorm_S8_uint, RHIResourceUsageBits::allow_depth_stencil, RHIMemoryType::default_);
        std::shared_ptr <RHITexture2D> colorImage = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::R8G8B8A8_unorm, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        std::shared_ptr <RHITexture2D> diffuseImage = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::R32G32B32A32_sfloat, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        std::shared_ptr <RHITexture2D> specularImage = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::R32G32B32A32_sfloat, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        std::shared_ptr <RHITexture2D> ambientImage = std::make_shared<RHITexture2D>(720, 720, 1, RHIFormat::R32G32B32A32_sfloat, RHIResourceUsageBits::allow_render_target | RHIResourceUsageBits::allow_transfer_src, RHIMemoryType::default_);
        texture_map["albedo"] = Resource{ albedo_image ,true };
        texture_map["normal"] = Resource{ normal_image ,true };
        texture_map["depth"] = Resource{ depthImage ,true };
        texture_map["color"] = Resource{ colorImage ,true };
        texture_map["diffuse"] = Resource{ diffuseImage ,true };
        texture_map["specular"] = Resource{ specularImage ,true };
        texture_map["ambient"] = Resource{ ambientImage ,true };

        output = colorImage;
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
        for (auto pass : passes)
        {
            pass->setupData();
        }

        for (auto& texture : texture_map)
        {
            IRHI::Get()->resourceBarrier(texture.second.resource_handle.get(), texture.second.resource_handle->state, RHIResourceState::transfer_dst);
            IRHI::Get()->clearImageResource(texture.second.resource_handle.get());
        }

        for (auto pass : passes)
        {
            pass->setResourceBarrier();
            pass->recordCommand();
        }
    }
}