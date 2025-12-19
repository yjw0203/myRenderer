#pragma once
#include "Engine/Utils/Public/Define.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Math/Public/Transform.h"

class Meta() RawBone : public MObject
{
    GENERATED_BODY();
public:
    Property()
    std::string m_name;
    Property()
    int         m_index;
    Property()
    Transform   m_binding_pose;
    Property()
    Matrix4x4   m_inverse_T_matrix;
    Property()
    int         m_parent_index;
};

class Meta() SkeletonAST : public MObject
{
    GENERATED_BODY();
public:
    Property()
    std::vector<RawBone> m_bones_map;
    Property()
    std::vector<int> m_skin_map;
};

class Meta() AnimationChannel : public MObject
{
    GENERATED_BODY();
public:
    Property()
    std::string m_name;
    Property()
    std::vector<Vector3>    m_position_keys;
    Property()
    std::vector<Quaternion> m_rotation_keys;
    Property()
    std::vector<Vector3>    m_scaling_keys;
};

class Meta() AnimationClipAST : public MObject
{
    GENERATED_BODY();
public:
    Property()
    int                           m_total_frame{ 0 };
    Property()
    float                         m_time_per_frame{ 0 };
    Property()
    std::vector<AnimationChannel> m_animation_channels;
};

class Meta() AnimationAST : public MObject
{
    GENERATED_BODY();
public:
    Property()
    AssetReferece<AnimationClipAST> m_clip;
};
