#pragma once
#include "Engine/Engine/Public/Framework/Render/IRenderModule.h"
#include<vector>
namespace yjw
{
    class Scene;
    class RenderSystem;
    class RenderModule : public IRenderModule
    {
    public:
        virtual void Startup();
        virtual void Shutdown();
        virtual void Tick(float deltaTime);

        virtual SceneInterface* CreateScene() override;

        virtual MaterialHandle LoadMaterial(const char* url) override;
        virtual void UnloadMaterial(MaterialHandle handle) override;

        virtual MeshHandle LoadMesh(const char* url) override;
        virtual void UnloadMesh(MeshHandle handle) override;

        virtual ViewProxy* CreateView(void* window) override;
        virtual void Destroy(ViewProxy* view) override;

        virtual void AttachView(ViewProxy* view) override;
        virtual void DettachView(ViewProxy* view) override;

    public:
        class MaterialManager* m_material_manager{};
        class PrimitiveManager* m_primitive_manager{};
    private:
        RenderSystem* m_render_system{};
        std::vector<class RViewProxy*> m_views;
    };

}