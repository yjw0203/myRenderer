#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"

void StaticMeshComponent::SetPrimitive(const char* url)
{
    m_mesh_url = url;
    if (m_scene)
    {
        RdScenePtr temp_scene = m_scene;
        DettachToScene();
        AttachToScene(temp_scene);
    }
}

const char* StaticMeshComponent::GetPrimitive()
{
    return m_mesh_url.c_str();
}

RdEntityPtr StaticMeshComponent::GetSceneEntity()
{
    return m_entity_handle;
}

void StaticMeshComponent::AttachToScene(RdScene* scene)
{
    m_scene = scene;
    m_entity_handle = rdAddEntity(scene);
    m_mesh_handle = rdCreateGeometry(GetPrimitive());
    rdUpdateEntityGeometry(scene, m_entity_handle, m_mesh_handle);
    rdUpdateEntityTransform(scene, m_entity_handle, GetTransform());
    rdUpdateEntityPickFlag(scene, m_entity_handle, GetActor()->GetActorId());
}

void StaticMeshComponent::DettachToScene()
{
    rdRemoveEntity(m_scene, m_entity_handle);
    rdDestroyGeometry(m_mesh_handle);
    m_scene = nullptr;
}
