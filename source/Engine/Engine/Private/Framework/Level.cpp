#include "Engine/Engine/Public/Render/Scene.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Engine/Public/Framework/Components/MeshComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"
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
}