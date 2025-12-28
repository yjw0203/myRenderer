#pragma once
#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"
#include "Engine/Oracle/Public/Render/OracleRenderWindow.h"
#include "Engine/Oracle/Public/Render/Font/OracleFontRenderHelper.h"

struct alignas(16) OracleRenderElementData
{
    LinearColour colour;
    Vector2 scale;
    Vector2 translate;
    Vector2 uv_offset;
    Vector2 uv_size;
};

struct alignas(16) InstanceData
{
    Vector4 instance_data;
};

struct OracleDrawOption
{
    bool with_index{};
    int first_instance{};
    int instance_count{};
    int first_vertex{};
    int vertex_count{};
};

struct OracleDrawItem
{
    rpi::RPIPrimitiveBinding primitive_binding{};
    int primitive_sub_id{0};
    rpi::RPIShader pixel_shader;
    rpi::RPIRenderPipeline pipeline;
    int push_constants[4] = { };
    rpi::RPIResourceSet resource_sets[rpi::RPIResourceSetType::count] = {};
    OracleDrawOption draw_option{};

    void Draw(rpi::RPIContext m_context);
};

class OracleBoxDrawElement;
class OracleTextDrawElement;
class OracleRenderer
{
public:
    static void StaticInitialize();
    void Initialize();
    void Render(OracleRenderWindow* RenderWindow, OcacleDrawElementList& DrawElementList);

private:
    void DrawBox(OracleBoxDrawElement* DrawElement);
    void DrawText(OracleTextDrawElement* DrawElement);
private:
    rpi::RPIContext m_context;

    rpi::RPIPrimitiveBinding m_box_primitive_binding{};
    rpi::RPIPrimitiveBinding m_text_primitive_binding{};
    rpi::RPIShader m_box_vertex_shader;
    rpi::RPIShader m_box_pixel_shader;
    rpi::RPIShader m_box_with_image_pixel_shader;
    rpi::RPIShader m_text_vertex_shader;
    rpi::RPIShader m_text_pixel_shader;
    rpi::RPIBuffer m_vb_position{};

    rpi::RPIRenderPipeline m_pipeline;

    rpi::RPIBuffer m_element_data_buffer{};
    rpi::RPIBuffer m_instance_data_buffer{};
    std::vector<OracleRenderElementData> m_element_data{};
    std::vector<InstanceData> m_instance_data{};
    rpi::RPIResourceSet m_element_resource_set{};

    rpi::RPIResourceSet m_ps_resource_sets[1000] = {};
    int m_current_allocate_ps_resource_set_id{0};

    std::vector<OracleDrawItem> m_draw_items;

    static OracleFontRenderHelper* s_font_helper;
};