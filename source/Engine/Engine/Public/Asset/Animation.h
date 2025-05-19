#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Math/Public/Transform.h"

namespace yjw
{
    Class(RawBone)
    {
    public:
        std::string m_name;
        int         m_index;
        Transform   m_binding_pose;
        Matrix4x4   m_inverse_T_matrix;
        int         m_parent_index;
    };

    CAsset(SkeletonAST)
    {
    public:
        std::vector<RawBone> m_bones_map;
        std::vector<int> m_skin_map;
    };

    Class(AnimationChannel)
    {
    public:
        std::string m_name;
        std::vector<Vector3>    m_position_keys;
        std::vector<Quaternion> m_rotation_keys;
        std::vector<Vector3>    m_scaling_keys;
    };

    CAsset(AnimationClipAST)
    {
    public:
        int                           m_total_frame{ 0 };
        float                         m_time_per_frame{ 0 };
        std::vector<AnimationChannel> m_animation_channels;
    };

    CAsset(AnimationAST)
    {
    public:
        AssetReferece<AnimationClipAST> m_clip;
    };

    void to_json(json& j, const yjw::Transform& obj);
    void from_json(const json& j, yjw::Transform& obj);
}

namespace glm
{
    void to_json(json& j, const yjw::Vector3& obj);
    void from_json(const json& j, yjw::Vector3& obj);
    void to_json(json& j, const yjw::Quaternion& obj);
    void from_json(const json& j, yjw::Quaternion& obj);
    void to_json(json& j, const yjw::Matrix4x4& obj);
    void from_json(const json& j, yjw::Matrix4x4& obj);
}