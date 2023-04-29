#include "yjw_rhi_draw_template.h"
#include "yjw_rhi_interface.h"

namespace rhi
{
    DefaultDrawTemplate::~DefaultDrawTemplate()
    {
        if (cache)
        {
            IRHI::Get()->deleteDrawTemplateCache(cache);
        }
    }

    void DefaultDrawTemplate::build()
    {
        if (cache)
        {
            IRHI::Get()->deleteDrawTemplateCache(cache);
        }
        cache = IRHI::Get()->buildDrawTemplateCache(this);
    }

    void DefaultDrawTemplate::draw()
    {
        if (cache == nullptr)
        {
            build();
        }
        IRHI::Get()->excuteDrawTemplate(this);
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setRasterizationState(RasterizationState state)
    {
        rasterizationState = state;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setColorBlendState(ColorBlendState state)
    {
        colorBlendState = state;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setVertexShaderView(RHIShaderView* shaderView)
    {
        vertexShaderView = shaderView;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setPixelShaderView(RHIShaderView* shaderView)
    {
        pixelShaderView = shaderView;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setRenderTarget(int num_rtvs, RHIResourceView* rtvs, RHIResourceView* dsv)
    {
        this->rtvs.resize(num_rtvs);
        for (int i = 0; i < num_rtvs; i++)
        {
            this->rtvs[i] = rtvs+i;
        }
        this->dsv = dsv;

        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setVertexBuffer(RHIResource* buffer, VertexLayout layout)
    {
        vertexBuffer = buffer;
        vertexLayout = layout;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setIndexBuffer(RHIResource* buffer)
    {
        indexBuffer = buffer;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setDraw(int vertexCount, int instanceCount, int firstVertex, int firstInstance)
    {
        drawOption.drawMode = DrawMode::draw;
        drawOption.vertexCount = vertexCount;
        drawOption.instanceCount = instanceCount;
        drawOption.firstVertex = firstVertex;
        drawOption.firstInstance = firstInstance;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setDrawIndex(int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
    {
        drawOption.drawMode = DrawMode::draw_index;
        drawOption.indexCount = indexCount;
        drawOption.instanceCount = instanceCount;
        drawOption.firstIndex = firstIndex;
        drawOption.firstInstance = firstInstance;
        drawOption.vertexOffset = vertexOffset;
        return this;
    }

    RasterizationState DefaultDrawTemplate::getRasterizationState()
    {
        return rasterizationState;
    }

    ColorBlendState DefaultDrawTemplate::getColorBlendState()
    {
        return colorBlendState;
    }

    RHIShaderView* DefaultDrawTemplate::getVertexShaderView()
    {
        return vertexShaderView;
    }

    RHIShaderView* DefaultDrawTemplate::getPixelShaderView()
    {
        return pixelShaderView;
    }

    std::vector<RHIResourceView*>* DefaultDrawTemplate::getRenderTargetViews()
    {
        return &rtvs;
    }

    RHIResourceView* DefaultDrawTemplate::getDepthStencilView()
    {
        return dsv;
    }

    VertexLayout& DefaultDrawTemplate::getVertexLayout()
    {
        return vertexLayout;
    }

    DrawOption& DefaultDrawTemplate::getDrawOption()
    {
        return drawOption;
    }

    RHIResource* DefaultDrawTemplate::getVertexBuffer()
    {
        return vertexBuffer;
    }

    RHIResource* DefaultDrawTemplate::getIndexBuffer()
    {
        return indexBuffer;
    }

}