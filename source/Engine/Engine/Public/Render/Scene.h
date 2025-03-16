#pragma once
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include <vector>

namespace yjw
{
    class MaterialInstance;
    struct RenderEntity
    {
        MaterialInstance* m_material{};
        rpi::RPIPrimitiveBinding m_primitive_binding{};
        rpi::RPIResourceBinding m_resource_binding{};
    };

    class PrimitiveComponent;
    class Scene
    {
    public:
        Scene();

        void AddPrimitiveComponent(PrimitiveComponent* mesh);
        void RemovePrimitiveComponent(PrimitiveComponent* mesh);

        std::vector<PrimitiveComponent*>& GetMeshes();

        void Update(float deltaTime);
    private:
        std::vector<PrimitiveComponent*> m_meshes;
        std::vector<Actor*> m_lights;
        std::vector<Actor*> m_cameras;

        std::vector<RenderEntity> m_render_entity;
        bool m_is_scene_dirty = false;
    };
}
