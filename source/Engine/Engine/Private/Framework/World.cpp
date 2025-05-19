#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include <queue>

namespace yjw
{
    void World::InitWorld()
    {
        m_level = new Level(this);
        m_scene = rdCreateScene();
    }

    void World::Tick(float deltaTime)
    {
        m_level->Update(deltaTime);
    }

    void World::DestroyWorld()
    {
        delete m_scene;
        delete m_level;
    }
}