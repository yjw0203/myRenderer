#pragma once
#include "Engine/Engine/Public/Framework/Render/Entity.h"
#include <string>

namespace yjw
{
    class SceneInterface
    {
    public:
        virtual void AddLight() {}
        virtual void AddCamera() {}

        virtual EntityHandle AddEntity() = 0;
        virtual void EraseEntity(EntityHandle handle) = 0;
        virtual void UpdateEntityMesh(EntityHandle entity, MeshHandle mesh) = 0;
        virtual void UpdateEntityOverrideMaterial(EntityHandle entity, const std::string& slot, MaterialHandle material) = 0;
    };
}