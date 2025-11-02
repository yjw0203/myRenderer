#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"

Actor::Actor()
{
    m_root_component.SetActor(this);
}

World* Actor::GetWorld()
{
    if (GetLevel())
    {
        return GetLevel()->GetWorld();
    }
    return nullptr;
}

Level* Actor::GetLevel()
{
    return m_level;
}

void Actor::OnLoaded()
{
    for (Component* component : m_components)
    {
        component->SetActor(this);
        component->OnLoaded();
    }
}

void Actor::AttachToLevel(Level* level)
{
    m_level = level;
    for (Component* component : m_components)
    {
        component->AttachToLevel(level);
    }

    for (Component* component : m_components)
    {
        component->AttachToScene(level->GetScene());
    }
}

void Actor::DettachToLevel()
{
    for (Component* component : m_components)
    {
        component->DettachToScene();
    }

    for (Component* component : m_components)
    {
        component->DettachToLevel();
    }

    m_level = nullptr;
}

void MeshActor::OnSpawned()
{
    Actor::OnSpawned();
    AddComponent<StaticMeshComponent>();
    StaticMeshComponent* mesh_component = GetComponent<StaticMeshComponent>();
    mesh_component->SetPrimitive(m_primitive_url.c_str());
}
