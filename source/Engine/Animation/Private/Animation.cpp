#include "Engine/Animation/Public/Animation.h"

BezierAnimation::BezierAnimation(std::shared_ptr<BezierAnimationKey> animation_key)
{
    m_animation_key = animation_key;
}

std::shared_ptr<SkeletonController> BezierAnimation::CreateSkeletonController()
{
    return std::make_shared<BezierAnimationController>(std::weak_ptr<BezierAnimationKey>(m_animation_key));
}

BezierAnimationController::BezierAnimationController(std::weak_ptr<BezierAnimationKey> animation)
{
    m_animation = animation;
}

void BezierAnimationController::Bind(Bone::List* bones)
{
    if (m_bones != bones)
    {
        m_bones = bones;
        if (bones)
        {
            m_node_index.resize(m_bones->size());
            for (int i = 0; i < m_node_index.size(); i++)
            {
                std::string name = (*bones)[i].m_name;
                std::shared_ptr<BezierAnimationKey> key = m_animation.lock();
                if (key && key->m_bone_index_map.count(name))
                {
                    m_node_index[i] = key->m_bone_index_map[name];
                }
                else
                {
                    m_node_index[i] = -1;
                }
            }
        }
    }
}

void BezierAnimationController::Evaluate(float time, float weight /*= 1.0f*/)
{
    std::shared_ptr<BezierAnimationKey> animation_key = m_animation.lock();
    if (m_bones && animation_key)
    {
        for (int index = 0; index < m_node_index.size(); index++)
        {
            int node_index = m_node_index[index];
            if (node_index == -1)
                continue;

            Bone& bone = (*m_bones)[index];
            std::vector<BezierAnimationKey::KeyNode>& nodes = animation_key->m_nodes[node_index];

            EvaluateBone(bone, nodes, time / 0.33, weight);
        }
    }
}

void BezierAnimationController::EvaluateBone(Bone& bone, std::vector<BezierAnimationKey::KeyNode>& nodes, float play_time, float weight)
{
    if (nodes.empty())
        return;

    //could be optimize
    int index = 0;
    while (index + 1 < nodes.size())
    {
        if (nodes[index].m_time <= play_time && nodes[index + 1].m_time > play_time)
            break;
        index++;
    }

    glm::vec3 vt;
    glm::quat q;
    if (index + 1 == nodes.size())
    {
        vt = nodes[index].m_translate;
        q = nodes[index].m_rotate;
    }
    else
    {
        vt = nodes[index].m_translate;
        q = nodes[index].m_rotate;
        if (play_time > nodes[index].m_time)
        {
            const auto& key0 = nodes[index];
            const auto& key1 = nodes[index + 1];

            float timeRange = float(key1.m_time - key0.m_time);
            float time = (play_time - float(key0.m_time)) / timeRange;
            float tx_x = key0.m_txBezier.FindBezierX(time);
            float ty_x = key0.m_tyBezier.FindBezierX(time);
            float tz_x = key0.m_tzBezier.FindBezierX(time);
            float rot_x = key0.m_rotBezier.FindBezierX(time);
            float tx_y = key0.m_txBezier.EvalY(tx_x);
            float ty_y = key0.m_tyBezier.EvalY(ty_x);
            float tz_y = key0.m_tzBezier.EvalY(tz_x);
            float rot_y = key0.m_rotBezier.EvalY(rot_x);

            vt = glm::mix(key0.m_translate, key1.m_translate, glm::vec3(tx_y, ty_y, tz_y));
            q = glm::slerp(key0.m_rotate, key1.m_rotate, rot_y);
        }
    }

    if (weight == 1.0f)
    {
        bone.m_anim_pose.m_rotate = q;
        bone.m_anim_pose.m_location = vt;
    }
    else
    {
        bone.m_anim_pose.m_rotate = glm::slerp(bone.m_anim_pose.m_rotate, q, weight);
        bone.m_anim_pose.m_location = glm::mix(bone.m_anim_pose.m_location, vt, weight);
    }
}

ClipAnimationController::ClipAnimationController(const char* url)
{
    m_clip.SetURL(url);
}

void ClipAnimationController::Bind(Bone::List* bones)
{
    if (m_bones == bones)
    {
        return;
    }
    AnimationClipAST* clip = m_clip.GetData();
    if (clip == nullptr)
    {
        return;
    }
    m_bones = bones;
    if (bones)
    {
        m_node_index.resize(m_bones->size());
        for (int i = 0; i < m_node_index.size(); i++)
        {
            m_node_index[i] = -1;
            std::string name = (*bones)[i].m_name;
            for (int j = 0; j < clip->m_animation_channels.size(); j++)
            {
                if (clip->m_animation_channels[j].m_name == name)
                {
                    m_node_index[i] = j;
                    break;
                }
            }
        }
    }
}
void ClipAnimationController::Evaluate(float time, float weight)
{
    AnimationClipAST* clip = m_clip.GetData();
    if (m_bones && clip && clip->m_total_frame > 0)
    {
        for (int index = 0; index < m_node_index.size(); index++)
        {
            int node_index = m_node_index[index];
            if (node_index == -1)
                continue;

            Bone& bone = (*m_bones)[index];
            AnimationChannel& nodes = clip->m_animation_channels[node_index];
            EvaluateBone(bone, nodes, time, clip->m_time_per_frame, weight);
        }
    }
}

void ClipAnimationController::EvaluateBone(Bone& bone, AnimationChannel& channel, float time, float time_per_frame, float weight)
{
    int low_frame = std::min((int)(time / time_per_frame), (int)(channel.m_position_keys.size() - 1));
    int high_frame = std::min(low_frame + 1, (int)(channel.m_position_keys.size() - 1));
    float lerp_ratio = time - low_frame * time_per_frame;

    Vector3 target_position = glm::mix(channel.m_position_keys[low_frame], channel.m_position_keys[high_frame], lerp_ratio);
    Vector3 target_scale = glm::mix(channel.m_scaling_keys[low_frame], channel.m_scaling_keys[high_frame], lerp_ratio);
    Quaternion target_rotation = glm::slerp(channel.m_rotation_keys[low_frame], channel.m_rotation_keys[high_frame], lerp_ratio);

    if (weight == 1.0f)
    {
        bone.m_anim_pose.m_rotate = target_rotation;
        bone.m_anim_pose.m_scale = target_scale;
        bone.m_anim_pose.m_location = target_position;
    }
    else
    {
        bone.m_anim_pose.m_rotate = glm::slerp(bone.m_anim_pose.m_rotate, target_rotation, weight);
        bone.m_anim_pose.m_scale = glm::mix(bone.m_anim_pose.m_scale, target_scale, weight);
        bone.m_anim_pose.m_location = glm::mix(bone.m_anim_pose.m_location, target_position, weight);
    }
}
