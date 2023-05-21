#include "yjw_scene.h"

namespace yjw
{
    std::vector<Entity> Scene::buildEntitys()
    {
        std::vector<Entity> res;
        for (std::weak_ptr<Model>& model : models)
        {
            std::shared_ptr<Model> model_ptr = model.lock();
            if (model_ptr)
            {
                for (Entity& entity : model_ptr->entitys)
                {
                    res.push_back(entity);
                }
            }
        }
        return res;
    }
}