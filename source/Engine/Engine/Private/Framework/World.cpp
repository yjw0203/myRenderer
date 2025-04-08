#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Render/Private/Mesh.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include <queue>

namespace yjw
{
    void World::InitWorld()
    {
        m_level = new Level(this);
        m_scene = GetModule<IRenderModule>()->CreateScene();
    }

    void World::DestroyWorld()
    {
        delete m_scene;
        delete m_level;
    }
}