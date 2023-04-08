#pragma once

#include "yjw_rhi_pipeline_state.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_resource_view.h"
#include <vector>

namespace rhi
{
    class DefaultDrawTemplateBuildCache{};

    class DefaultDrawTemplate
    {
    public:
        DefaultDrawTemplate() {};
        ~DefaultDrawTemplate();
        void build();
        void draw();
        DefaultDrawTemplateBuildCache* cache = nullptr;
        DefaultDrawTemplate* setRasterizationState(RasterizationState state);
        DefaultDrawTemplate* setColorBlendState(ColorBlendState state);
        DefaultDrawTemplate* setVertexShader(RHIShader* shader);
        DefaultDrawTemplate* setPixelShader(RHIShader* shader);
        DefaultDrawTemplate* setRenderTarget(int num_rtv, RHIResourceView* rtvs, RHIResourceView* dsv);

        RasterizationState getRasterizationState();
        ColorBlendState getColorBlendState();
        RHIShader* getVertexShader();
        RHIShader* getPixelShader();
        std::vector<RHIResourceView*>* getRenderTargetViews();
        RHIResourceView* getDepthStencilView();
    private:
        RasterizationState rasterizationState;
        ColorBlendState colorBlendState;
        RHIShader* vertexShader = nullptr;
        RHIShader* pixelShader = nullptr;
        std::vector<RHIResourceView*> rtvs;
        RHIResourceView* dsv = nullptr;
    };

}
