#pragma once
#include "Engine/Animation/Public/Bone.h"
#include "Engine/Animation/Public/AnimationData.h"
#include "Engine/Engine/Public/Asset/Animation.h"
#include <vector>
#include <memory>

namespace yjw
{
    class SkeletonController;
    class Skeleton
    {
    public:
        ~Skeleton();
        bool BuildSkeleton(const char* url);
        bool BuildSkeleton(const std::vector<RawBone>& m_bones_map, const std::vector<int>& skin_map);
        void LoadController(std::shared_ptr<SkeletonController> controller);
        void Update(float time);
        Bone::List* GetBones();
        const std::vector<Matrix4x4>& GetBoneMatrices();
        
        void BeginUpdateAnimation();
        void EndUpdateAnimation();
    private:
        Bone::List m_bones;
        std::shared_ptr<SkeletonController> m_controller;
        std::vector<int> m_skin_map;
        std::vector<Matrix4x4> m_bone_matrices;
    };

    class SkeletonController
    {
    public:
        virtual void Bind(Bone::List* bones) = 0;
        virtual void Evaluate(float time, float weight = 1.0f) = 0;
    };
}