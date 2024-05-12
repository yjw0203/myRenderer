#pragma once
#include "yjw_light.h"
#include "Model/yjw_model.h"
#include <vector>
#include <memory>

namespace yjw
{
    struct RenderEntity
    {
        GPUModel::GPUMesh* mesh{};
        Material* material{};
    };

    class Scene
    {
    public:
        std::vector<std::weak_ptr<Light> > lights;
        std::vector<std::weak_ptr<Model> > models;

        std::vector<RenderEntity> buildEntitys();
    };
}