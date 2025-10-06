#include "Engine/Engine/Public/Framework/System/AnimationSystem.h"
#include "Engine/Engine/Public/Framework/Components/SkeletonComponent.h"
#include "Engine/Engine/Public/Framework/Components/AnimationComponent.h"
#include "Engine/Engine/Public/Framework/Actor.h"

namespace yjw
{
    void AnimationSystem::Update(float deltaTime)
    {
        for (Actor* actor : GetLevel()->GetActors())
        {
            if (!actor)
            {
                continue;
            }
            SkeletonComponent* skeleton_component = actor->GetComponent<SkeletonComponent>();
            AnimationComponent* animation_component = actor->GetComponent<AnimationComponent>();
            if (skeleton_component)
            {
                skeleton_component->GetSkeleton()->BeginUpdateAnimation(); 
                if (animation_component)
                {
                    animation_component->Update(deltaTime);
                    animation_component->ControlSkeleton(skeleton_component->GetSkeleton());
                }
                skeleton_component->GetSkeleton()->EndUpdateAnimation();
                const std::vector<Matrix4x4>& bone_matrix = skeleton_component->GetSkeleton()->GetBoneMatrices();
                rdUpdateEntitySkeletalMatrix(actor->GetWorld()->GetScene(), actor->GetSceneEntity(), bone_matrix.data(), bone_matrix.size());
            }
        }
    }
}