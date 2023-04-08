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

    DefaultDrawTemplate* DefaultDrawTemplate::setVertexShader(RHIShader* shader)
    {
        vertexShader = shader;
        return this;
    }

    DefaultDrawTemplate* DefaultDrawTemplate::setPixelShader(RHIShader* shader)
    {
        pixelShader = shader;
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

    RasterizationState DefaultDrawTemplate::getRasterizationState()
    {
        return rasterizationState;
    }

    ColorBlendState DefaultDrawTemplate::getColorBlendState()
    {
        return colorBlendState;
    }

    RHIShader* DefaultDrawTemplate::getVertexShader()
    {
        return vertexShader;
    }

    RHIShader* DefaultDrawTemplate::getPixelShader()
    {
        return pixelShader;
    }

    std::vector<RHIResourceView*>* DefaultDrawTemplate::getRenderTargetViews()
    {
        return &rtvs;
    }

    RHIResourceView* DefaultDrawTemplate::getDepthStencilView()
    {
        return dsv;
    }

}