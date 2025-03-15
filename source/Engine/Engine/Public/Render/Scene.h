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

    class Scene
    {
    public:
        Scene();

        void AddMesh(Mesh* mesh);
        void RemoveMesh(Mesh* mesh);

        std::vector<Mesh*>& GetMeshes();

        std::vector<RenderEntity> buildEntitys();
        const std::vector<RenderEntity>& getEntitys();
        void Update(float deltaTime);
    private:
        std::vector<Mesh*> m_meshes;
        std::vector<Actor*> m_lights;
        std::vector<Actor*> m_cameras;

        std::vector<RenderEntity> m_render_entity;
        bool m_is_scene_dirty = false;
    };
}
