#include "Engine/Engine/Public/Render/Scene.h"
#include "Engine/Engine/Public/Framework/Components/PrimitiveComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"
#include <queue>

namespace yjw
{
    Scene::Scene()
    {
        
    }

    void Scene::AddPrimitiveComponent(PrimitiveComponent* mesh)
    {
        m_meshes.push_back(mesh);
    }

    void Scene::RemovePrimitiveComponent(PrimitiveComponent* mesh)
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

    std::vector<PrimitiveComponent*>& Scene::GetMeshes()
    {
        return m_meshes;
    }

    void Scene::Update(float deltaTime)
    {

    }

}