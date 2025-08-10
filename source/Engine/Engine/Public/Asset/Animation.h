#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Math/Public/Transform.h"
#include "Generate/Public/Engine/Animation/Public/Animation.generate.h"

namespace yjw
{
    Class(RawBone) : public MObject
    {
        GENERATED_BODY();
    public:
        std::string m_name;
        int         m_index;
        Transform   m_binding_pose;
        Matrix4x4   m_inverse_T_matrix;
        int         m_parent_index;
    };

    CAsset(SkeletonAST) : public MObject
    {
        GENERATED_BODY();
    public:
        std::vector<RawBone> m_bones_map;
        std::vector<int> m_skin_map;
    };

    Class(AnimationChannel) : public MObject
    {
        GENERATED_BODY();
    public:
        std::string m_name;
        std::vector<Vector3>    m_position_keys;
        std::vector<Quaternion> m_rotation_keys;
        std::vector<Vector3>    m_scaling_keys;
    };

    CAsset(AnimationClipAST) : public MObject
    {
        GENERATED_BODY();
    public:
        int                           m_total_frame{ 0 };
        float                         m_time_per_frame{ 0 };
        std::vector<AnimationChannel> m_animation_channels;
    };

    CAsset(AnimationAST) : public MObject
    {
        GENERATED_BODY();
    public:
        AssetReferece<AnimationClipAST> m_clip;
    };

}
