#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/View.h"
#include "Engine/Render/Public/RenderAPI.h"

using namespace rpi;
class PickPass
{
public:
    void Initialize();
    void Destroy();
    void AttachView(RdView* view);
    void Submit();
    void AddPendingHitRequest(const char* group_name, const RdHitRequestStruct& request);
    void GetProcessedHitRequest(const char* group_name, std::vector<RdHitRequestStruct>& proccessed_request);
private:
    void SubmitOpacue(DrawItem* item);
private:
    RPITexture m_pick_output_texture{};
    RPITexture m_pick_staging_texture{};
    RPITexture m_pick_depth_texture{};
    RPIRenderPass m_render_pass{};

    RPIRenderPipeline m_pipeline{};
    RPIShader m_ps{};

    RPIContext m_context{};

    RdView* m_view{};

    std::vector<std::pair<std::string, RdHitRequestStruct>> m_pending_request;
    std::unordered_map<std::string, std::vector<RdHitRequestStruct>> m_processed_request;
};
