#pragma once
#include "Bone.h"
#include "AnimationData.h"
#include <vector>
#include <memory>

namespace yjw
{
    class SkeletonController;
    class Skeleton
    {
    public:
        ~Skeleton();
        bool BuildSkeleton(const SkeletonData& skeleton_data);
        void LoadController(std::shared_ptr<SkeletonController> controller);
        void Update(float time);
        const Bone::List& GetBones();
        const std::vector<Matrix4x4>& GetBoneMatrices();
        
    private:
        void BeginUpdateAnimation();
        void EndUpdateAnimation();
        Bone::List m_bones;
        std::shared_ptr<SkeletonController> m_controller;
        std::vector<Matrix4x4> m_bone_matrices;
    };

    class SkeletonController
    {
    public:
        virtual void Bind(Bone::List* bones) = 0;
        virtual void Evaluate(float time, float weight = 1.0f) = 0;
    };
}