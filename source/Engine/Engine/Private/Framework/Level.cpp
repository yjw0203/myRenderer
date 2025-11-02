#include "Engine/Render/Private/Scene.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Engine/Public/Framework/System/AnimationSystem.h"
#include <queue>

Level::Level()
{
        
}

Level::~Level()
{
    DestroyLevel();
}

void Level::InitLevel()
{
    DestroyLevel();
    m_scene = rdCreateScene();
}

void Level::DestroyLevel()
{
    delete m_scene;
}

void Level::OnLoaded()
{
    InitLevel();
    for (Actor* actor : m_actors)
    {
        actor->OnLoaded();
        AttachActor(actor);
    }
    for (System* system : m_systems)
    {
        system->OnLoaded();
        AttachSystem(system);
    }
}

const std::vector<Actor*>& Level::GetActors()
{
    return m_actors;
}

void Level::Update(float deltaTime)
{
    rdResetSkeletal(GetScene());

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

void Level::AttachToWorld(World* world)
{
    m_world = world;
}

void Level::DettachToWorld()
{
    m_world = nullptr;
}

void Level::AttachActor(Actor* actor)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i] == actor)
        {
            if (m_actors[i]->GetLevel() == nullptr)
            {
                m_actors[i]->AttachToLevel(this);
            }
            return;
        }
    }
    actor->AttachToLevel(this);
    m_actors.push_back(actor);
}

void Level::DettachActor(Actor* actor)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i] == actor)
        {
            actor->DettachToLevel();
            m_actors[i] = m_actors.back();
            m_actors.resize(m_actors.size() - 1);
        }
    }
}

void Level::AttachSystem(System* system)
{
    for (int i = 0; i < m_systems.size(); i++)
    {
        if (m_systems[i] == system)
        {
            if (m_systems[i]->GetLevel() == nullptr)
            {
                m_systems[i]->AttachToLevel(this);
            }
            return;
        }
    }
    system->AttachToLevel(this);
    m_systems.push_back(system);
}

void Level::DettachSystem(System* system)
{
    for (int i = 0; i < m_systems.size(); i++)
    {
        if (m_systems[i] == system)
        {
            system->DettachToLevel();
            m_systems[i] = m_systems.back();
            m_systems.resize(m_systems.size() - 1);
        }
    }
}
