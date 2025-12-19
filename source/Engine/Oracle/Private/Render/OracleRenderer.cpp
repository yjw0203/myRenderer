#include "Engine/Oracle/Public/Render/OracleRenderer.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleBoxDrawElement.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "projectInfo.h"

using namespace rpi;

void OracleRenderer::Initialize()
{
    RPIInit();
    m_context = RPICreateContext();

    m_box_vertex_shader = RPICreateShader(RPIShaderType::vertex, SHADER_FILE(OracleUI.hlsl), "BoxVS");
    m_box_pixel_shader = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(OracleUI.hlsl), "BoxPS");
    m_box_with_image_pixel_shader = RPICreateShader(RPIShaderType::fragment, SHADER_FILE(OracleUI.hlsl), "BoxPSWithImage");
    
    m_primitive_binding = RPICreatePrimitiveBinding(m_box_vertex_shader);
    glm::vec2 vertex[6] = { glm::vec2(0,0),glm::vec2(0,1),glm::vec2(1,0),glm::vec2(1,0),glm::vec2(0,1),glm::vec2(1,1) };
    m_vb_position = RPICreateGpuVertexBuffer(sizeof(vertex));
    RPIUpdateBuffer(m_vb_position, vertex, 0, sizeof(vertex));
    m_primitive_binding.SetVertexBuffer(RHIName("POSITION"), m_vb_position);
    
    RPIRenderPipelineDescriptor pipelineDesc1 = RPIGetDefaultRenderPipeline();
    pipelineDesc1.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_no_depth_no_stencil);
    pipelineDesc1.color_blend_state[0] = RPIGetColorBlendState(RPIColorBlendStateType::default_alpha_blend);
    m_pipeline = RPICreateRenderPipeline(pipelineDesc1);


    rpi::ShaderReflect reflect{};
    rpi::ShaderReflect::SSBO buffer{};
    buffer.m_set = rpi::RPIGetResourceSetIDByType(rpi::RPIResourceSetType::entity);
    buffer.m_name = "ElementDataBuffer";
    buffer.m_binding = 0;
    reflect.m_ssbos.push_back(buffer);
    m_element_resource_set = rpi::RPICreateResourceSet(rpi::RPIResourceSetType::entity, &reflect);

    int max_element = 1000; // to be remove
    m_element_data_buffer = rpi::RPICreateUploadBuffer(max_element * sizeof(OracleRenderElementData));
    m_element_resource_set.SetBuffer("ElementDataBuffer", m_element_data_buffer);

    for (int i = 0; i < max_element; i++)
    {
        m_ps_resource_sets[i] = RPICreateResourceSet(RPIResourceSetType::ps, m_box_with_image_pixel_shader->GetShaderReflect());
    }
}

void OracleRenderer::Render(OracleRenderWindow* renderWindow, OcacleDrawElementList& DrawElementList)
{
    m_element_data.clear();
    for (OracleDrawElement* DrawElement : DrawElementList.GetArray())
    {
        OracleRenderElementData data;
        data.colour = ((OracleBoxDrawElement*)DrawElement)->m_background_colour;
        data.scale = Vector2(DrawElement->m_transform.transform[0][0], DrawElement->m_transform.transform[1][1]);
        data.translate = Vector2(DrawElement->m_transform.local_position.x, DrawElement->m_transform.local_position.y);
        m_element_data.push_back(data);
    }

    rpi::RPIUpdateBuffer(m_element_data_buffer, m_element_data.data(), 0, m_element_data.size() * sizeof(OracleRenderElementData));

    RPICmdClearBackBuffer(m_context, renderWindow->GetRPIWindow());
    RPIRenderPass render_pass = renderWindow->GetRPIWindow().swapchain->GetCurrentRenderPass();
    RPICmdBeginRenderPass(m_context, render_pass);
    for (int index = 0; index < DrawElementList.GetArray().size(); index++)
    {
        OracleDrawElement* DrawElement = DrawElementList.GetArray()[index];

        if (((OracleBoxDrawElement*)DrawElement)->m_image.IsVaild())
        {
            m_ps_resource_sets[index].SetTexture("image", ((OracleBoxDrawElement*)DrawElement)->m_image.GetResource());
            RPICmdSetResourceSet(m_context, RPIResourceSetType::ps, m_ps_resource_sets[index]);
            RPICmdSetRenderPipeline(m_context, m_pipeline, m_primitive_binding, m_box_with_image_pixel_shader);
        }
        else
        {
            RPICmdSetRenderPipeline(m_context, m_pipeline, m_primitive_binding, m_box_pixel_shader);
        }

        int flag[4] = { index, 0,0,0 };
        RPICmdPushConstants(m_context, flag, 0, sizeof(int) * 4);
        RPICmdSetResourceSet(m_context, RPIResourceSetType::entity, m_element_resource_set);
        RPICmdSetPrimitiveBinding(m_context, m_primitive_binding, 0);
        RPICmdDraw(m_context, 6, 1, 0, 0);
    }
    RPICmdEndPass(m_context);

    RPISubmit(m_context);
    rpi::RPIPresent(m_context, renderWindow->GetRPIWindow());
}