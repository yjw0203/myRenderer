#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Math/Public/Transform.h"

class Meta() RawBone : public MObject
{
    GENERATED_BODY();
public:
    Meta()
    std::string m_name;
    Meta()
    int         m_index;
    Meta()
    Transform   m_binding_pose;
    Meta()
    Matrix4x4   m_inverse_T_matrix;
    Meta()
    int         m_parent_index;
};

class Meta() SkeletonAST : public MObject
{
    GENERATED_BODY();
public:
    Meta()
    std::vector<RawBone> m_bones_map;
    Meta()
    std::vector<int> m_skin_map;
};

class Meta() AnimationChannel : public MObject
{
    GENERATED_BODY();
public:
    Meta()
    std::string m_name;
    Meta()
    std::vector<Vector3>    m_position_keys;
    Meta()
    std::vector<Quaternion> m_rotation_keys;
    Meta()
    std::vector<Vector3>    m_scaling_keys;
};

class Meta() AnimationClipAST : public MObject
{
    GENERATED_BODY();
public:
    Meta()
    int                           m_total_frame{ 0 };
    Meta()
    float                         m_time_per_frame{ 0 };
    Meta()
    std::vector<AnimationChannel> m_animation_channels;
};

class Meta() AnimationAST : public MObject
{
    GENERATED_BODY();
public:
    Meta()
    AssetReferece<AnimationClipAST> m_clip;
};
