#pragma once
#include "Engine/Animation/Public/Skeleton.h"
#include <unordered_map>

class SkeletonAnimation
{
public:
    virtual std::shared_ptr<SkeletonController> CreateSkeletonController() = 0;
};

class BezierAnimationKey
{
public:
    struct KeyNode
    {
        std::string m_name;
        int32_t     m_time{};
        glm::vec3   m_translate;
        glm::quat   m_rotate;

        Bezier  m_txBezier;
        Bezier  m_tyBezier;
        Bezier  m_tzBezier;
        Bezier  m_rotBezier;
    };

    std::unordered_map<std::string, int> m_bone_index_map;
    std::vector<std::vector<KeyNode>> m_nodes;
};

class BezierAnimation : public SkeletonAnimation
{
public:
    BezierAnimation(std::shared_ptr<BezierAnimationKey> animation_key);
    virtual std::shared_ptr<SkeletonController> CreateSkeletonController() override;
private:
    std::shared_ptr<BezierAnimationKey> m_animation_key;
};

class BezierAnimationController : public SkeletonController
{
public:
    BezierAnimationController(std::weak_ptr<BezierAnimationKey> animation);
    virtual void Bind(Bone::List* bones) override;
    virtual void Evaluate(float time, float weight = 1.0f) override;
private:
    void EvaluateBone(Bone& bone, std::vector<BezierAnimationKey::KeyNode>& nodes, float time, float weight = 1.0f);
private:
    Bone::List* m_bones{};
    std::vector<int> m_node_index;
    std::weak_ptr<BezierAnimationKey> m_animation;
};

class ClipAnimationController : public SkeletonController
{
public:
    ClipAnimationController(const char* url);
    virtual void Bind(Bone::List* bones) override;
    virtual void Evaluate(float time, float weight = 1.0f) override;
private:
    void EvaluateBone(Bone& bone, AnimationChannel& channel, float time, float time_per_frame, float weight = 1.0f);
private:
    Bone::List* m_bones{};
    std::vector<int> m_node_index;
    Asset<AnimationClipAST> m_clip;
};
