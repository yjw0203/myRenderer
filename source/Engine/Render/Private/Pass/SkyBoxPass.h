#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"

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
    RPIRenderPipeline m_pipeline{};
    RPIShader m_vs{};
    RPIShader m_ps{};
    RPIPrimitiveBinding m_primitive_binding{};
    RPIResourceSet m_custom_ps_resource_set{};
};
