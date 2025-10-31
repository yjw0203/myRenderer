#pragma once
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Render/Private/Primitive.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
#include "Engine/Render/Public/RenderAPI.h"
namespace yjw
{
    class Meta() StaticMeshComponent : public PrimitiveComponent
    {
        GENERATED_BODY();
    public:
        void SetPrimitive(const char* url);
        
        virtual const char* GetPrimitive() override;
        RdEntityPtr GetSceneEntity();
        virtual void AttachToScene(RdScene* scene);
        virtual void DettachToScene();
    private:
        Meta()
        std::string m_mesh_url{};

        RdGeometryPtr m_mesh_handle{};
        RdScenePtr m_scene{};
        RdEntityPtr m_entity_handle{};
    };
}
