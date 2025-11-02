#pragma once
#include "Engine/Animation/Public/Animation.h"
#include "Engine/Engine/Public/Asset/Animation.h"

class VMD
{
public:
    void Load(const std::string& url);
    void ConvertBezierAnimationKeyToAnimationClip(std::shared_ptr<BezierAnimationKey> bezier_animation_key, AnimationClipAST& animation_clip);

    std::shared_ptr<BezierAnimationKey> m_bezier_animation_key;
    AnimationClipAST m_animation_clip;
};
