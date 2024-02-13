#pragma once
#include "yjw_light.h"
#include "yjw_model.h"
#include <vector>
#include <memory>

namespace yjw
{
    class Scene
    {
    public:
        std::vector<std::weak_ptr<Light> > lights;
        std::vector<std::weak_ptr<Model> > models;

        std::vector<Entity> buildEntitys();
    };
}