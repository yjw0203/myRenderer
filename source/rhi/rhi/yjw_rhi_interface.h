#pragma once
namespace rhi
{
    struct RHIResourceDesc;
    class RHIResourceLocation;
    class RHIResourceViewLocation;
    struct RHIShaderDesc;
    class RHIResource;
    class RHIShaderLocation;
    class DefaultDrawTemplate;
    class DefaultDrawTemplateBuildCache;
    enum ResourceViewType;
    enum RHIFormat;

    struct CreateInfo
    {
        void* window;
    };

    class IRHI
    {
    public:
        static IRHI* Get();

        virtual void initialize(CreateInfo info) = 0;

        virtual void beginFrame() = 0;
        virtual void endFrame(RHIResource* present_texture) = 0;

        //resource
        virtual RHIResourceLocation* createResource(const RHIResourceDesc& desc) = 0;
        virtual void deleteResource(RHIResourceLocation*& location) = 0;

        //resource_view
        virtual RHIResourceViewLocation* createResourceView(ResourceViewType type, const RHIResource* resource, RHIFormat view_format) = 0;
        virtual void deleteResourceView(RHIResourceViewLocation*& location) = 0;

        //shader
        virtual RHIShaderLocation* createShader(const RHIShaderDesc& desc) = 0;
        virtual void deleteShader(RHIShaderLocation*& location) = 0;

        //draw template
        virtual void excuteDrawTemplate(const DefaultDrawTemplate* draw_template) = 0;
        virtual DefaultDrawTemplateBuildCache* buildDrawTemplateCache(DefaultDrawTemplate* draw_template) = 0;
        virtual void deleteDrawTemplateCache(DefaultDrawTemplateBuildCache*& draw_template_cache) = 0;
    };
}