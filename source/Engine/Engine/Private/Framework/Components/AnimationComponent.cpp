#include "Engine/Engine/Public/Framework/Components/AnimationComponent.h"

namespace yjw
{
    AnimationComponent::AnimationComponent()
    {

    }

    AnimationComponent::~AnimationComponent()
    {

    }

    void AnimationComponent::LoadAnimation(const char* url)
    {
        Asset<AnimationAST> animation_ast(url);
        AnimationClipAST* clip = animation_ast.GetData()->m_clip.GetData();
        m_total_time = clip->m_total_frame * clip->m_time_per_frame;
        m_skeleton_controller = std::make_shared<ClipAnimationController>(animation_ast.GetData()->m_clip.m_url.c_str());
    }

    void AnimationComponent::Update(float deltaTime)
    {
        m_play_time = m_play_time + deltaTime;
        if (m_play_time > m_total_time)
        {
            m_play_time = m_play_time - (int)(m_play_time / m_total_time) * m_total_time;
        }
    }

    void AnimationComponent::ControlSkeleton(Skeleton* skeleton)
    {
        if (m_skeleton_controller)
        {
            m_skeleton_controller->Bind(skeleton->GetBones());
            m_skeleton_controller->Evaluate(m_play_time);
        }

        if (m_ik_controller)
        {
            m_ik_controller->Bind(skeleton->GetBones());
            m_ik_controller->Evaluate(m_play_time);
        }
    }
}