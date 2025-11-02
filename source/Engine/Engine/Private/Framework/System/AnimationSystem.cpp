#include "Engine/Engine/Public/Framework/System/AnimationSystem.h"
#include "Engine/Engine/Public/Framework/Components/SkeletonComponent.h"
#include "Engine/Engine/Public/Framework/Components/AnimationComponent.h"
#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"
#include "Engine/Engine/Public/Framework/Actor.h"
#include "Engine/Engine/Public/Framework/Level.h"

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
        StaticMeshComponent* mesh_component = actor->GetComponent<StaticMeshComponent>();
        if (skeleton_component && skeleton_component->GetSkeleton() && mesh_component)
        {
            skeleton_component->GetSkeleton()->BeginUpdateAnimation(); 
            if (animation_component)
            {
                animation_component->Update(deltaTime);
                animation_component->ControlSkeleton(skeleton_component->GetSkeleton());
            }
            skeleton_component->GetSkeleton()->EndUpdateAnimation();
            const std::vector<Matrix4x4>& bone_matrix = skeleton_component->GetSkeleton()->GetBoneMatrices();
            rdUpdateEntitySkeletalMatrix(actor->GetWorld()->GetScene(), mesh_component->GetSceneEntity(), bone_matrix.data(), bone_matrix.size());
        }
    }
}
