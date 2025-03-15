#include "Engine/Engine/Public/Render/Scene.h"
#include "Engine/Engine/Public/Framework/Components/MeshComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"
#include <queue>

namespace yjw
{
    Scene::Scene()
    {
        
    }

    void Scene::AddMesh(Mesh* mesh)
    {
        m_meshes.push_back(mesh);
    }

    void Scene::RemoveMesh(Mesh* mesh)
    {
        for (int i = 0; i < m_meshes.size(); i++)
        {
            if (m_meshes[i] == mesh)
            {
                m_meshes[i] = m_meshes.back();
                m_meshes.resize(m_meshes.size() - 1);
            }
        }
    }

    std::vector<Mesh*>& Scene::GetMeshes()
    {
        return m_meshes;
    }

    std::vector<RenderEntity> Scene::buildEntitys()
    {
        std::vector<RenderEntity> res;
        for (Mesh* mesh : m_meshes)
        {
            RenderEntity entity{};
            entity.m_primitive_binding = mesh->GetPrimitive()->GetPrimitiveBinding();
            entity.m_resource_binding = mesh->GetMaterialInstance()->GetResourceBinding();
            entity.m_material = mesh->GetMaterialInstance();
            if (!entity.m_primitive_binding.IsNull() && !entity.m_resource_binding.IsNull())
            {
                res.push_back(entity);
            }
        }

        return res;
    }

    const std::vector<RenderEntity>& Scene::getEntitys()
    {
        if (m_is_scene_dirty)
        {
            m_render_entity = buildEntitys();
        }
        return m_render_entity;
    }

    void Scene::Update(float deltaTime)
    {

    }

}