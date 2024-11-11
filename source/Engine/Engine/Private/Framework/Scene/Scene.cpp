#include "Engine/Framework/Scene/Scene.h"
#include "Engine/Framework/Components/MeshComponent.h"
#include "Engine/Framework/Mesh/Mesh.h"
#include <queue>

namespace yjw
{
    Scene::Scene()
    {
        
    }

    std::vector<RenderEntity> Scene::buildEntitys()
    {
        std::vector<RenderEntity> res;
        for (Actor* actor : m_actors)
        {
            std::queue<Component*> Q;
            Q.push(actor->GetRootComponent());
            while (!Q.empty())
            {
                Component* component = Q.front();
                Q.pop();
                MeshComponent* mesh_component = component->CastTo<MeshComponent>();
                if (mesh_component)
                {
                    RenderEntity entity{};
                    entity.m_primitive_binding = mesh_component->GetPrimitive()->GetPrimitiveBinding();
                    entity.m_resource_binding = mesh_component->GetMaterialInstance()->GetResourceBinding();
                    entity.m_material = mesh_component->GetMaterialInstance();
                    res.push_back(entity);
                }
                for (Component* sub_component : component->GetSubComponents())
                {
                    Q.push(sub_component);
                }
            }
        }

        return res;
    }

    const std::vector<RenderEntity>& Scene::getEntitys()
    {
        if (m_is_scene_dirty)
        {
            m_render_entity = buildEntitys();
        }
        return m_render_entity;
    }

    const std::vector<Actor*>& Scene::GetActors()
    {
        return m_actors;
    }

    void Scene::addBox()
    {
        Actor* actor = new Actor("Box");
        MaterialInstance* material_instance = new MaterialInstance(&g_simple_mesh_pbr_material);
        rpi::RPITexture texture = rpi::RPICreateTexture2DFromFile(RESOURCE_FILE(cao/tex/髮1.png));
        material_instance->SetTexture("albedoTex", texture);
        Mesh* mesh = new Mesh(&g_box_primitive, material_instance);
        MeshComponent* mesh_component = new MeshComponent();
        mesh_component->SetMesh(mesh);
        actor->GetRootComponent()->AddSubComponent(mesh_component);
        m_actors.push_back(actor);

        Actor* actor1 = new Actor("Box2");
        Mesh* mesh1 = new Mesh(&g_box_primitive, material_instance);
        MeshComponent* mesh_component1 = new MeshComponent();
        mesh_component1->SetMesh(mesh);
        actor1->GetRootComponent()->AddSubComponent(mesh_component1);
        m_actors.push_back(actor1);

        m_is_scene_dirty = true;
    }
}