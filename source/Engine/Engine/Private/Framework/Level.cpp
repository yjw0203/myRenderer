#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Engine/Public/Framework/System/AnimationSystem.h"
#include <queue>

namespace yjw
{
    Level::Level(World* world)
    {
        m_world = world;

        System* animationSystem = new AnimationSystem();
        animationSystem->RegisterToLevel(this);
        m_systems.push_back(animationSystem);
    }

    const std::vector<Actor*>& Level::GetActors()
    {
        return m_actors;
    }

    void Level::Update(float deltaTime)
    {
        rdResetSkeletal(m_world->GetScene());

        for (System* system : m_systems)
        {
            system->Update(deltaTime);
        }
    }

    Actor* Level::GetActorById(int id)
    {
        for (Actor* actor : m_actors)
        {
            if (actor->GetActorId() == id)
            {
                return actor;
            }
        }
        return nullptr;
    }
}