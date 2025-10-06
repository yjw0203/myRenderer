#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Render/Public/RenderAPI.h"
#include "Engine/Utils/Public/Serialize/Archive.h"

namespace yjw
{
    class System;
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
            Actor* actor = new T(std::forward<Args>(args)...);
            actor->SetName(name);
            actor->OnSpawned();
            actor->SetActorId(m_actors.size() + 1);
            m_actors.push_back(actor);
            actor->SetWorld(m_world);
            
            if (StaticMeshComponent* mesh_component = (StaticMeshComponent*)actor->m_components[0])
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
        std::vector<System*> m_systems;
        World* m_world{};
    };
}