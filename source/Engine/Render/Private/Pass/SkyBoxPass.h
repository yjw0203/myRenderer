#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"

namespace yjw
{
    using namespace rpi;
    class SkyBoxPass
    {
    public:
        void Initialize();
        void Destroy();
        void Submit(RPIContext context);
        void LoadResource(const char* sky_box_path);

    private:
        RPITexture m_sky_box{};

        RPIBuffer m_vertex_buffer{};
        RPIBuffer m_index_buffer{};
        RPIPipeline m_pipeline{};
        RPIShader m_vs{};
        RPIShader m_ps{};
        RPIResourceBinding m_resource_binding{};
        RPIPrimitiveBinding m_primitive_binding{};
    };
}