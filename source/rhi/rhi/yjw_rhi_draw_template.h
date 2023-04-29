#pragma once

#include "yjw_rhi_pipeline_state.h"
#include "yjw_rhi_shader.h"
#include "yjw_rhi_resource_view.h"
#include <vector>

namespace rhi
{
    class DefaultDrawTemplateBuildCache{};

    enum class DrawMode
    {
        none = 0,
        draw,
        draw_index,
        draw_indirect,
        draw_index_indirect
    };

    struct DrawOption
    {
        DrawMode drawMode;
        int vertexCount;
        int instanceCount;
        int firstVertex;
        int firstInstance;
        int indexCount;
        int firstIndex;
        int vertexOffset;//only use when draw index
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
        DefaultDrawTemplate* setIndexBuffer(RHIResource* buffer);
        DefaultDrawTemplate* setDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance);
        DefaultDrawTemplate* setDrawIndex(int indexCount, int instanceCount, int firstIndex,int vertexOffset, int firstInstance);

        RasterizationState getRasterizationState();
        ColorBlendState getColorBlendState();
        RHIShaderView* getVertexShaderView();
        RHIShaderView* getPixelShaderView();
        std::vector<RHIResourceView*>* getRenderTargetViews();
        RHIResourceView* getDepthStencilView();
        VertexLayout& getVertexLayout();
        DrawOption& getDrawOption();
        RHIResource* getVertexBuffer();
        RHIResource* getIndexBuffer();
    private:
        RasterizationState rasterizationState;
        ColorBlendState colorBlendState;
        RHIShaderView* vertexShaderView = nullptr;
        RHIShaderView* pixelShaderView = nullptr;
        std::vector<RHIResourceView*> rtvs;
        RHIResourceView* dsv = nullptr;
        RHIResource* vertexBuffer = nullptr;
        RHIResource* indexBuffer = nullptr;
        VertexLayout vertexLayout;
        DrawOption drawOption;



    };

}
