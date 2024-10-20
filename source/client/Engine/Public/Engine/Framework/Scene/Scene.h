#pragma once
#include "Engine/Framework/Actor/Actor.h"
#include "RHI/rpi/yjw_rpi.h"
#include <vector>

namespace yjw
{
    class MaterialInstance;
    struct RenderEntity
    {
        MaterialInstance* m_material{};
        rpi::RPIPrimitiveBinding m_primitive_binding{};
        rpi::RPIResourceBinding m_resource_binding{};
    };

    class Scene
    {
    public:
        Scene();
        std::vector<RenderEntity> buildEntitys();
    private:
        std::vector<Actor*> m_actors;
    };
}
