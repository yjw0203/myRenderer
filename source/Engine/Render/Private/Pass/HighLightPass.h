#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/View.h"
#include "Engine/Render/Public/RenderAPI.h"

using namespace rpi;
class HighLightPass
{
public:
    void Initialize();
    void Destroy();
    void AttachView(RdView* view);
    void SubmitHighLightObject(RPIContext context);
    void HighLightPostProcess(RPIContext context);
private:
    void SubmitOpacue(DrawItem* item);
private:
    RPIShader m_highlight_mask_ps{};
    RPITexture m_highlight_mask_normal{};
    RPITexture m_highlight_mask_depth{};
    RPIRenderPipeline m_highlight_mask_pipeline{};
    RPIRenderPass m_highlight_mask_render_pass{};

    RPIPrimitiveBinding m_post_process_Primitive_binding{};
    RPIBuffer m_post_process_vb{};
    RPIShader m_highlight_post_process_vs{};
    RPIShader m_highlight_post_process_ps{};
    RPIRenderPipeline m_highlight_post_process_pipeline{};
    RPIResourceSet m_high_light_post_process_ps_resource_set{};

    RdView* m_view{};

    RPIContext m_current_context{};
};
