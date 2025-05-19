#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Render/Public/RenderAPI.h"
namespace yjw
{
    class Actor;
    class Level
    {
    public:
        Level(World* world);

        World* GetWorld() { return m_world; }

        const std::vector<Actor*>& GetActors();
        void Update(float deltaTime);

        template<typename T, typename... Args>
        Actor* SpawnActor(const char* name, Args&&... args)
        {
            Entity& entity = m_ecs_manager.GetEntityManager().CreateEntity();
            Actor* actor = new T(std::forward<Args>(args)...);
            entity.SetParent(actor);
            actor->SetName(name);
            actor->SetEntity(entity);
            actor->OnSpawned();
            actor->SetActorId(m_actors.size() + 1);
            m_actors.push_back(actor);
            actor->SetWorld(m_world);
            if (StaticMeshComponent* mesh_component = actor->GetEntity().GetComponent<StaticMeshComponent>())
            {
                RdEntityPtr entity_handle = rdAddEntity(GetWorld()->GetScene());
                actor->SetSceneEntity(entity_handle);
                RdGeometryPtr mesh_handle = rdCreateGeometry(mesh_component->GetPrimitive());
                rdUpdateEntityGeometry(GetWorld()->GetScene(), entity_handle, mesh_handle);
                rdUpdateEntityTransform(GetWorld()->GetScene(), entity_handle, actor->GetTransform());
                rdUpdateEntityPickFlag(GetWorld()->GetScene(), entity_handle, actor->GetActorId());
            }

            return actor;
        }

        Actor* GetActorById(int id);
    private:
        std::vector<Actor*> m_actors;

        ECSManager m_ecs_manager;

        World* m_world{};
    };
}