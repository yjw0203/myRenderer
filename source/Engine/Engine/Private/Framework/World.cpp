#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include <queue>
#include "Engine/Engine/Public/Framework/System/AnimationSystem.h"
#include "Engine/Utils/Public/Serialize/FileArchive.h"

namespace yjw
{
    void World::InitWorld()
    {
        m_level = new Level();
        m_level->InitLevel();
        m_level->AttachSystem(new AnimationSystem());
        AttachLevel(m_level);
    }

    void World::Tick(float deltaTime)
    {
        if (m_level)
        {
            m_level->Update(deltaTime);
        }
    }

    void World::DestroyWorld()
    {
        DettachLevel();
        delete m_level;
    }

    Level* World::GetLevel() { return m_level; }
    RdScene* World::GetScene() 
    {
        if (m_level)
        {
            return m_level->GetScene(); 
        }
        return nullptr;
    }

    void World::AttachLevel(Level* level)
    {
        if (level)
        {
            m_level = level;
            m_level->AttachToWorld(this);
        }
    }

    void World::DettachLevel()
    {
        if (m_level)
        {
            m_level->DettachToWorld();
            m_level = nullptr;
        }
    }

    void World::LoadLevel(const char* url)
    {
        DettachLevel();
        FileReaderArchive Ar(url);
        Ar << m_level;
        Ar.close();
        m_level->OnLoaded();
        AttachLevel(m_level);
    }

    void World::SaveLevel(const char* url)
    {
        FileWriterArchive Ar(url);
        Ar << m_level;
        Ar.close();
    }
}