#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Render/Public/RenderAPI.h"
#include "Engine/Utils/Public/Serialize/Archive.h"
#include "Engine/Utils/Public/Object/Object.h"
#include "Engine/Engine/Public/Framework/System/System.h"

namespace yjw
{
    class System;
    class Actor;
    
    class Meta() Level : public MObject
    {
        GENERATED_BODY()
    public:
        Level();
        virtual ~Level();

        void InitLevel();
        void DestroyLevel();
        void OnLoaded();

        World* GetWorld() { return m_world; }

        const std::vector<Actor*>& GetActors();
        void Update(float deltaTime);

        template<typename T, typename... Args>
        Actor* SpawnActor(const char* name, Args&&... args)
        {
            Actor* actor = new T(std::forward<Args>(args)...);
            actor->SetName(name);
            actor->OnSpawned();
            AttachActor(actor);
            return actor;
        }

        Actor* GetActorById(int id);
        RdScene* GetScene() { return m_scene; }
        void AttachToWorld(World* world);
        void DettachToWorld();

        void AttachActor(Actor* actor);
        void DettachActor(Actor* actor);

        void AttachSystem(System* system);
        void DettachSystem(System* system);

    private:
        Meta()
        std::vector<Actor*> m_actors;
        Meta()
        std::vector<System*> m_systems;
        
        World* m_world{};
        RdScene* m_scene = nullptr;
    };
}