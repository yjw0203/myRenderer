#include "yjw_scene.h"

namespace yjw
{
    std::vector<RenderEntity> Scene::buildEntitys()
    {
        std::vector<RenderEntity> res;
        for (std::weak_ptr<Model>& model : models)
        {
            std::shared_ptr<Model> model_ptr = model.lock();
            if (model_ptr)
            {
                for (int entity_id = 0; entity_id < model_ptr->m_gpu_model->GetEntityCount(); entity_id++)
                {
                    res.push_back(RenderEntity{ model_ptr->m_gpu_model->GetGPUMesh(entity_id),model_ptr->m_gpu_model->GetMaterial(entity_id) });
                }
            }
        }
        return res;
    }
}