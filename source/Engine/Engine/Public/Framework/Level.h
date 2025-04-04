#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Render/Private/Scene.h"
namespace yjw
{
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
            Entity entity = m_ecs_manager.GetEntityManager().CreateEntity();
            Actor* actor = new T(std::forward<Args>(args)...);
            actor->SetName(name);
            actor->SetEntity(entity);
            actor->OnSpawned();
            m_actors.push_back(actor);
            if (StaticMeshComponent* mesh_component = actor->GetEntity().GetComponent<StaticMeshComponent>())
            {
                GetWorld()->GetScene()->AddPrimitiveComponent(mesh_component);
            }

            return actor;
        }
    private:
        std::vector<Actor*> m_actors;

        ECSManager m_ecs_manager;

        World* m_world{};
    };
}