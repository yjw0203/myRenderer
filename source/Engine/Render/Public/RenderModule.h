#pragma once
#include "Engine/Engine/Public/Framework/Render/IRenderModule.h"

namespace yjw
{
    class Scene;
    class RenderSystem;
    class RenderModule : public IRenderModule
    {
    public:
        virtual void Startup();
        virtual void Shutdown();
        virtual void AttachScene(SceneInterface* scene) override;
        virtual void AttachUI(rhi::ImGuiUI* ui) override;
        virtual void Tick(float deltaTime);

        virtual SceneInterface* CreateScene() override;

        virtual MaterialHandle LoadMaterial(const char* url) override;
        virtual void UnloadMaterial(MaterialHandle handle) override;

        virtual MeshHandle LoadMesh(const char* url) override;
        virtual void UnloadMesh(MeshHandle handle) override;

    public:
        class MaterialManager* m_material_manager{};
        class PrimitiveManager* m_primitive_manager{};
    private:
        RenderSystem* m_render_system{};
    };

}