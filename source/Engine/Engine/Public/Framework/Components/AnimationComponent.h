#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Animation/Public/Animation.h"

namespace yjw
{
    class Meta() AnimationComponent : public Component
    {
        GENERATED_BODY();
    public:
        AnimationComponent();
        ~AnimationComponent();
        void LoadAnimation(const char* url);
        void Update(float deltaTime);
        void ControlSkeleton(Skeleton* skeleton);

    private:
        std::shared_ptr<SkeletonController> m_skeleton_controller;
        std::shared_ptr<SkeletonController> m_ik_controller;

        float m_play_time = 0.0f;
        float m_total_time = 0.0f;
    };
}