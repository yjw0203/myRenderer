#pragma once
#include "Engine/Engine/Public/Framework/Render/Entity.h"
#include "Engine/RHI/Public/externs/imgui/yjw_rhi_imgui_window.h"
#include <string>

namespace yjw
{
    class SceneInterface;
    class ViewProxy
    {
    public:
        virtual ~ViewProxy() {}
        virtual void AttachScene(SceneInterface* scene) = 0;
        virtual void AttachUI(class rhi::ImGuiUI* ui) = 0;
    };

    class SceneInterface
    {
    public:
        virtual void AddLight() {}
        virtual void AddCamera() {}

        virtual EntityHandle AddEntity() = 0;
        virtual void EraseEntity(EntityHandle handle) = 0;
        virtual void UpdateEntityMesh(EntityHandle entity, MeshHandle mesh) = 0;
        virtual void UpdateEntityOverrideMaterial(EntityHandle entity, const std::string& slot, MaterialHandle material) = 0;
        virtual void UpdateEntityPickFlag(EntityHandle entity, int pick_flag[4]) = 0;
        virtual void UpdateEntityRenderMask(EntityHandle entity, RenderMaskBits maskBit, bool enable) = 0;
    };
}