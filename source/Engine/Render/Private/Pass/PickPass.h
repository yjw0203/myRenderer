#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"

namespace yjw
{
    using namespace rpi;
    class PickPass
    {
    public:
        void Initialize();
        void Destroy();
        void Submit(RPIContext context);

    private:
        RPITexture m_pick_texture{};

        RPIRenderPipeline m_pipeline{};
        RPIShader m_ps{};
        RPIResourceSet m_global_resource_set{};
        RPIResourceSet m_custom_vs_resource_set{};
        RPIResourceSet m_custom_ps_resource_set{};
    };
}