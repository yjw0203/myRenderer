#include "Engine/Oracle/Public/Render/OracleRenderer.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleBoxDrawElement.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleTextDrawElement.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "projectInfo.h"

using namespace rpi;


OracleFontRenderHelper* OracleRenderer::s_font_helper = nullptr;
void OracleRenderer::StaticInitialize()
{
    RPIInit();
    s_font_helper = new OracleFontRenderHelper();
}

void OracleRenderer::Initialize()
{
    m_context = RPICreateContext();

    m_box_vertex_shader = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(OracleUI.hlsl), "BoxVS");
    m_box_pixel_shader = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(OracleUI.hlsl), "BoxPS");
    m_box_with_image_pixel_shader = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(OracleUI.hlsl), "BoxPSWithImage");
    m_text_vertex_shader = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(OracleUI.hlsl), "TextVS");
    m_text_pixel_shader = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(OracleUI.hlsl), "TextPS");
    
    glm::vec2 vertex[6] = { glm::vec2(0,0),glm::vec2(0,1),glm::vec2(1,0),glm::vec2(1,0),glm::vec2(0,1),glm::vec2(1,1) };
    m_vb_position = RPICreateGpuVertexBuffer(sizeof(vertex));
    RPIUpdateBuffer(m_vb_position, vertex, 0, sizeof(vertex));

    m_text_primitive_binding = RPICreatePrimitiveBinding(m_text_vertex_shader);
    m_text_primitive_binding.SetVertexBuffer(RHIName("POSITION"), m_vb_position);

    m_box_primitive_binding = RPICreatePrimitiveBinding(m_box_vertex_shader);
    m_box_primitive_binding.SetVertexBuffer(RHIName("POSITION"), m_vb_position);
    
    RPIRenderPipelineDescriptor pipelineDesc1 = RPIGetDefaultRenderPipeline();
    pipelineDesc1.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_no_depth_no_stencil);
    pipelineDesc1.color_blend_state[0] = RPIGetColorBlendState(RPIColorBlendStateType::default_alpha_blend);
    m_pipeline = RPICreateRenderPipeline(pipelineDesc1);

    rpi::ShaderReflect reflect{};
    rpi::ShaderReflect::SSBO buffer{};
    buffer.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
    buffer.m_name = "InstanceDataBuffer";
    buffer.m_binding = 0;
    rpi::ShaderReflect::SSBO buffer1{};
    buffer1.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
    buffer1.m_name = "ElementDataBuffer";
    buffer1.m_binding = 1;
    rpi::ShaderReflect::SSBO buffer2{};
    buffer2.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
    buffer2.m_name = "GlyphInfoBuffer";
    buffer2.m_binding = 2;
    reflect.m_ssbos.push_back(buffer);
    reflect.m_ssbos.push_back(buffer1);
    reflect.m_ssbos.push_back(buffer2);
    m_element_resource_set = rpi::RPICreateResourceSet(rpi::RPIResourceSetType::entity, &reflect);

    int max_element = 1000; // to be remove
    m_element_data_buffer = rpi::RPICreateUploadBuffer(max_element * sizeof(OracleRenderElementData));
    m_instance_data_buffer = rpi::RPICreateUploadBuffer(max_element * sizeof(InstanceData));
    m_element_resource_set.SetBuffer("ElementDataBuffer", m_element_data_buffer);
    m_element_resource_set.SetBuffer("InstanceDataBuffer", m_instance_data_buffer);
    m_element_resource_set.SetBuffer("GlyphInfoBuffer", m_instance_data_buffer);

    for (int i = 0; i < max_element; i++)
    {
        m_ps_resource_sets[i] = RPICreateResourceSet(RPIResourceSetType::ps, m_box_with_image_pixel_shader->GetShaderReflect());
    }
}

void OracleDrawItem::Draw(rpi::RPIContext context)
{
    RPICmdSetRenderPipeline(context, pipeline, primitive_binding, pixel_shader);
    RPICmdPushConstants(context, push_constants, 0, sizeof(int) * 4);
    for (int i = 0; i < RPIResourceSetType::count; i++)
    {
        RPICmdSetResourceSet(context, (RPIResourceSetType)i, resource_sets[i]);
    }
    RPICmdSetPrimitiveBinding(context, primitive_binding, primitive_sub_id);

    if (draw_option.with_index)
    {
        RPICmdDrawIndex(context, draw_option.first_instance, draw_option.instance_count);
    }
    else
    {
        RPICmdDraw(context, draw_option.vertex_count, draw_option.instance_count, draw_option.first_vertex, draw_option.first_instance);
    }
}

void OracleRenderer::Render(OracleRenderWindow* renderWindow, OcacleDrawElementList& DrawElementList)
{
    m_current_allocate_ps_resource_set_id = 0;
    m_draw_items.clear();

    m_element_data.clear();
    m_instance_data.clear();
    for (int index = 0; index < DrawElementList.GetArray().size(); index++)
    {
        OracleDrawElement* DrawElement = DrawElementList.GetArray()[index];
        if (DrawElement->GetTypeID() == OracleBoxDrawElement::GetStaticTypeID())
        {
            DrawBox((OracleBoxDrawElement*)DrawElement);
        }
        else if(DrawElement->GetTypeID() == OracleTextDrawElement::GetStaticTypeID())
        {
            DrawText((OracleTextDrawElement*)DrawElement);
        }
    }

    rpi::RPIUpdateBuffer(m_instance_data_buffer, m_instance_data.data(), 0, m_instance_data.size() * sizeof(InstanceData));
    rpi::RPIUpdateBuffer(m_element_data_buffer, m_element_data.data(), 0, m_element_data.size() * sizeof(OracleRenderElementData));

    RPICmdClearBackBuffer(m_context, renderWindow->GetRPIWindow());
    RPIRenderPass render_pass = renderWindow->GetRPIWindow().swapchain->GetCurrentRenderPass();
    RPICmdBeginRenderPass(m_context, render_pass);
    for (OracleDrawItem& draw_item : m_draw_items)
    {
        draw_item.Draw(m_context);
    }
    RPICmdEndPass(m_context);
    RPISubmit(m_context);
    rpi::RPIPresent(m_context, renderWindow->GetRPIWindow());
}

void OracleRenderer::DrawBox(OracleBoxDrawElement* DrawElement)
{
    OracleDrawItem draw_item = OracleDrawItem{};
    draw_item.primitive_binding = m_box_primitive_binding;
    draw_item.primitive_sub_id = 0;
    draw_item.pipeline = m_pipeline;

    if (DrawElement->m_image.IsVaild())
    {
        draw_item.pixel_shader = m_box_with_image_pixel_shader;
        int allocate_ps_resource_set_id = m_current_allocate_ps_resource_set_id++;
        m_ps_resource_sets[allocate_ps_resource_set_id].SetTexture("image", DrawElement->m_image.GetResource());
        draw_item.resource_sets[RPIResourceSetType::ps] = m_ps_resource_sets[allocate_ps_resource_set_id++];
    }
    else
    {
        draw_item.pixel_shader = m_box_pixel_shader;
    }
    draw_item.resource_sets[RPIResourceSetType::entity] = m_element_resource_set;

    draw_item.draw_option.with_index = false;
    draw_item.draw_option.first_vertex = 0;
    draw_item.draw_option.vertex_count = 6;
    draw_item.draw_option.instance_count = 1;
    draw_item.draw_option.first_instance = (int)m_instance_data.size();

    InstanceData instance_data{};
    instance_data.instance_data.x = (int)m_element_data.size();
    m_instance_data.push_back(instance_data);

    OracleRenderElementData data;
    data.colour = DrawElement->m_background_colour;
    data.scale = Vector2(DrawElement->m_transform.transform[0][0], DrawElement->m_transform.transform[1][1]);
    data.translate = Vector2(DrawElement->m_transform.local_position.x, DrawElement->m_transform.local_position.y);
    data.uv_offset = Vector2(0, 0);
    data.uv_size = Vector2(1, 1);
    m_element_data.push_back(data);

    m_draw_items.push_back(draw_item);
}

void OracleRenderer::DrawText(OracleTextDrawElement* DrawElement)
{
    std::vector<OracleGlyphRenderInfo> glyph_infos;
    s_font_helper->RegisterText(glyph_infos, DrawElement->m_text, DrawElement->m_size, DrawElement->m_font_path);

    float font_height = DrawElement->m_size / DrawElement->m_resolution.y;
    float linegap = (DrawElement->m_size + 2) / DrawElement->m_resolution.y;
    Vector2 cursor_pos = Vector2(0, 0);
    float box_width = DrawElement->m_transform.GetScale().x;
    float box_height = DrawElement->m_transform.GetScale().y;
    Vector2 resolution = DrawElement->m_resolution;
    int instance_count = 0;
    int first_instance = m_instance_data.size();
    for (OracleGlyphRenderInfo& info : glyph_infos)
    {
        float width = info.width / resolution.x;
        float height = info.height / resolution.y;
        float advance = info.advance / resolution.x;
        float bearingX = info.bearingX / resolution.x;
        float bearingY = info.bearingY / resolution.y;
        if (cursor_pos.x + advance > box_width && cursor_pos.x > 0 || info.unicode == '\n')
        {
            cursor_pos.x = 0;
            cursor_pos.y = cursor_pos.y + linegap;
        }

        InstanceData instance_data{};
        instance_data.instance_data.x = (int)m_element_data.size();
        m_instance_data.push_back(instance_data);

        OracleRenderElementData data;
        data.colour = DrawElement->m_colour;
        data.scale = Vector2(width, height);
        data.translate = DrawElement->m_transform.local_position + cursor_pos + Vector2(bearingX, font_height - bearingY);
        data.uv_offset = info.uv_offset;
        data.uv_size = info.uv_size;
        m_element_data.push_back(data);

        instance_count++;

        cursor_pos.x += advance;
    }

    OracleDrawItem draw_item = OracleDrawItem{};
    draw_item.primitive_binding = m_text_primitive_binding;
    draw_item.primitive_sub_id = 0;
    draw_item.pipeline = m_pipeline;
    draw_item.pixel_shader = m_text_pixel_shader;

    draw_item.resource_sets[RPIResourceSetType::entity] = m_element_resource_set;
    int allocate_ps_resource_set_id = m_current_allocate_ps_resource_set_id++;
    m_ps_resource_sets[allocate_ps_resource_set_id].SetTexture("image", s_font_helper->GetFontTexture());
    draw_item.resource_sets[RPIResourceSetType::ps] = m_ps_resource_sets[allocate_ps_resource_set_id++];

    draw_item.draw_option.with_index = false;
    draw_item.draw_option.first_vertex = 0;
    draw_item.draw_option.vertex_count = 6;
    draw_item.draw_option.instance_count = instance_count;
    draw_item.draw_option.first_instance = first_instance;

    m_draw_items.push_back(draw_item);
}