#pragma once
#include <memory>
#include "yjw_rhi_header.h"
#include <unordered_map>
#include <unordered_set>

namespace rhi
{

    struct RHIPipelineLocation {};;
    class RHIPipelineDestoryer;
    class RHIGraphicsPipelineCreator;
    class RHIComputePipelineCreator;
    class RHIPipeline
    {
        struct SimplePairHash {
            std::uint64_t operator()(const std::pair<int, int>& p) const {
                return ((uint64_t)p.first << 32) + p.second;
            }
        };
    public:
        static RHIGraphicsPipelineCreator NewGraphicsPipline();
        static RHIComputePipelineCreator NewComputePipline();
        RasterizationState rasterizationState;
        DepthStencilState depthStencilState;
        ColorBlendState colorBlendState;

        std::shared_ptr<RHIShader> vs;
        std::string vs_entry;
        std::shared_ptr<RHIShader> ps;
        std::string ps_entry;
        std::shared_ptr<RHIShader> cs;
        std::string cs_entry;

        int num_vertex_binding = 0;
        std::unordered_map< std::pair<int, int>, RHIFormat, SimplePairHash > vertex_bindings;

        int num_set = 0;
        std::vector<std::unordered_map< std::pair<int, int>, int, SimplePairHash >> uniform_buffer = std::vector<std::unordered_map< std::pair<int, int>, int, SimplePairHash >>(RHIShaderType::shader_type_count);
        std::vector < std::unordered_set< std::pair<int, int>, SimplePairHash >> texture_srv = std::vector < std::unordered_set< std::pair<int, int>, SimplePairHash >>(RHIShaderType::shader_type_count);

        int num_color_attachment = 0;
        std::unordered_map<int, RHIFormat> color_attachments;

        bool has_depth_stencil_attachment = false;
        RHIFormat depth_stencil_attachment;
        std::shared_ptr<RHIPipelineLocation> location;
        virtual ~RHIPipeline();
        RHIPipeline() {}
    private:

    };

    template<class T>
    class RHIPipelineCreator
    {
        friend class RHIPipeline;
    public:
        RHIPipelineCreator()
        {
            pipeline = std::make_shared<RHIPipeline>();
        }
        operator std::shared_ptr<RHIPipeline>()
        {
            pipeline->location = IRHI::Get()->createPipeline(pipeline);
            return std::move(pipeline);
        }

        T& RASTERIZATION_STATE(RasterizationState state) { pipeline->rasterizationState = state;  return *static_cast<T*>(this); }
        T& DEPTH_STENCIL_STATE(DepthStencilState state) { pipeline->depthStencilState = state;  return *static_cast<T*>(this); }
        T& COLOR_BLEND_STATE(ColorBlendState state) { pipeline->colorBlendState = state;  return *static_cast<T*>(this); }

        template<RHIShaderType stage,int set, int binding>
        T& UNIFORM_BUFFER(int size) { pipeline->uniform_buffer[stage][std::make_pair(set, binding)] = size; pipeline->num_set = max(pipeline->num_set, set + 1); return *static_cast<T*>(this); }
        
        template<RHIShaderType stage,int set, int binding>
        T& TEXTURE_SRV() { pipeline->texture_srv[stage].insert(std::make_pair(set, binding)); pipeline->num_set = max(pipeline->num_set, set + 1); return *static_cast<T*>(this); }

        template<int slot>
        T& COLOR_ATTACHMENT(RHIFormat format)
        {
           pipeline->num_color_attachment = max(pipeline->num_color_attachment, slot + 1);
           pipeline->color_attachments[slot] = format;
           return *static_cast<T*>(this);
        }

        T& DEETH_STENCIL_ATTACHMENT(RHIFormat format)
        {
            pipeline->has_depth_stencil_attachment = true;
            pipeline->depth_stencil_attachment = format;
            return *static_cast<T*>(this);
        }


    protected:
        std::shared_ptr<RHIPipeline> pipeline;
    };

    class RHIGraphicsPipelineCreator : public RHIPipelineCreator<RHIGraphicsPipelineCreator>
    {
    public:
        RHIGraphicsPipelineCreator& VS(std::shared_ptr<RHIShader> shader, const char* entry) { pipeline->vs = shader; pipeline->vs_entry = std::string(entry); return *this; }
        RHIGraphicsPipelineCreator& PS(std::shared_ptr<RHIShader> shader, const char* entry) { pipeline->ps = shader; pipeline->ps_entry = std::string(entry); return *this; }

        template<int binding,int location>
        RHIGraphicsPipelineCreator& VERTEX_BINDING(RHIFormat format) { pipeline->vertex_bindings[std::make_pair(binding, location)] = format; pipeline->num_vertex_binding = max(pipeline->num_vertex_binding, binding + 1); return *this; }
    };

    class RHIComputePipelineCreator : public RHIPipelineCreator<RHIComputePipelineCreator>
    {
    public:
        RHIComputePipelineCreator& CS(std::shared_ptr<RHIShader> shader, const char* entry) { pipeline->cs = shader; pipeline->cs_entry = std::string(entry); return *this; }
    };

    class RHIPipelineDestoryer
    {
    public:
        RHIPipelineDestoryer(RHIPipeline* pipeline) { this->pipeline = pipeline; }
        virtual ~RHIPipelineDestoryer(){ IRHI::Get()->destoryPipeline(pipeline->location); }
    private:
        RHIPipeline* pipeline;
    };
}