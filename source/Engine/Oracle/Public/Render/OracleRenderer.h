#pragma once
#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"
#include "Engine/Oracle/Public/Render/OracleRenderWindow.h"

struct alignas(16) OracleRenderElementData
{
    LinearColour colour;
    Vector2 scale;
    Vector2 translate;
};

class OracleRenderer
{
public:
    static void StaticInitialize();
    void Initialize();
    void Render(OracleRenderWindow* RenderWindow, OcacleDrawElementList& DrawElementList);

private:
    rpi::RPIContext m_context;

    rpi::RPIPrimitiveBinding m_primitive_binding{};
    rpi::RPIShader m_box_vertex_shader;
    rpi::RPIShader m_box_pixel_shader;
    rpi::RPIShader m_box_with_image_pixel_shader;
    rpi::RPIBuffer m_vb_position{};

    rpi::RPIRenderPipeline m_pipeline;

    rpi::RPIResourceSet m_element_resource_set{};
    rpi::RPIBuffer m_element_data_buffer{};
    std::vector<OracleRenderElementData> m_element_data{};

    rpi::RPIResourceSet m_ps_resource_sets[1000] = {};
};