#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Animation/Public/Skeleton.h"

namespace yjw
{
    class SkeletonComponent : public IComponent
    {
    public:
        SkeletonComponent();
        ~SkeletonComponent();
        void LoadSkeleton(const char* url);

        Skeleton* GetSkeleton();

    private:
        Skeleton* m_skeleton = nullptr;
    };

}