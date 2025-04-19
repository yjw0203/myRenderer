#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include <queue>

namespace yjw
{
    Level::Level(World* world)
    {
        m_world = world;
    }

    const std::vector<Actor*>& Level::GetActors()
    {
        return m_actors;
    }

    void Level::Update(float deltaTime)
    {
        m_ecs_manager.GetSystemManager().Update(deltaTime);
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