#pragma once
#include <vulkan/vulkan.h>
#include "rhi/rhi/yjw_rhi_interface.h"

namespace rhi
{

    class IVulkanRHI : public IRHI
    {
    public:
        virtual void initialize(CreateInfo info) override;

        virtual void beginFrame() override;
        virtual void endFrame(RHIResource* present_texture) override;

        //resource
        virtual RHIResourceLocation* createResource(const RHIResourceDesc& desc) override;
        virtual void deleteResource(RHIResourceLocation*& location) override;

        //resource_view
        virtual RHIResourceViewLocation* createResourceView(ResourceViewType type, const RHIResource* resource, RHIFormat view_format) override;
        virtual void deleteResourceView(RHIResourceViewLocation*& location) override;

        //shader
        virtual RHIShaderLocation* createShader(const RHIShaderDesc& desc) override;
        virtual void deleteShader(RHIShaderLocation*& location) override;

        //draw template
        virtual void excuteDrawTemplate(const DefaultDrawTemplate* draw_template) override;
        virtual DefaultDrawTemplateBuildCache* buildDrawTemplateCache(DefaultDrawTemplate* draw_template) override;
        virtual void deleteDrawTemplateCache(DefaultDrawTemplateBuildCache*& draw_template_cache) override;
    };
}