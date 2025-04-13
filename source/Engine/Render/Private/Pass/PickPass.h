#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/View.h"
#include "Engine/Engine/Public/Framework/Render/RenderHit.h"

namespace yjw
{
    using namespace rpi;
    class PickPass
    {
    public:
        void Initialize();
        void Destroy();
        void AttachView(RView* view);
        void Submit();
        void AddPendingHitRequest(const char* group_name, const RenderHitRequest& request);
        void GetProcessedHitRequest(const char* group_name, std::vector<RenderHitRequest>& proccessed_request);
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

        RView* m_view{};

        std::vector<std::pair<std::string, RenderHitRequest>> m_pending_request;
        std::unordered_map<std::string, std::vector<RenderHitRequest>> m_processed_request;
    };
}