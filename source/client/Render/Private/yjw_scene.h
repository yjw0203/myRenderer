#pragma once
#include "yjw_light.h"
#include "Model/yjw_model.h"
#include "Model/yjw_material.h"
#include <vector>
#include <memory>

namespace yjw
{
    struct RenderEntity
    {
        GPUModel::GPUMesh* mesh{};
        MaterialInstance* material{};
    };

    class Scene
    {
    public:
        std::vector<std::weak_ptr<Light> > lights;
        std::vector<std::weak_ptr<Model> > models;

        std::vector<RenderEntity> buildEntitys();
    };
}