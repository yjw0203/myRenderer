#pragma once

#include "yjw_rhi_pipeline_state.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_resource_view.h"
#include <vector>

namespace rhi
{
    class DefaultDrawTemplateBuildCache{};

    struct DrawOption
    {
        int vertexCount;
        int instanceCount;
        int firstVertex;
        int firstInstance;
    };

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
        DefaultDrawTemplate* setVertexShaderView(RHIShaderView* shader);
        DefaultDrawTemplate* setPixelShaderView(RHIShaderView* shader);
        DefaultDrawTemplate* setRenderTarget(int num_rtv, RHIResourceView* rtvs, RHIResourceView* dsv);
        DefaultDrawTemplate* setVertexBuffer(RHIResource* buffer, VertexLayout layout);
        DefaultDrawTemplate* setDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);

        RasterizationState getRasterizationState();
        ColorBlendState getColorBlendState();
        RHIShaderView* getVertexShaderView();
        RHIShaderView* getPixelShaderView();
        std::vector<RHIResourceView*>* getRenderTargetViews();
        RHIResourceView* getDepthStencilView();
        VertexLayout& getVertexLayout();
        DrawOption& getDrawOption();
        RHIResource* getVertexBuffer();
    private:
        RasterizationState rasterizationState;
        ColorBlendState colorBlendState;
        RHIShaderView* vertexShaderView = nullptr;
        RHIShaderView* pixelShaderView = nullptr;
        std::vector<RHIResourceView*> rtvs;
        RHIResourceView* dsv = nullptr;
        RHIResource* vertexBuffer = nullptr;
        VertexLayout vertexLayout;
        DrawOption drawOption;



    };

}
