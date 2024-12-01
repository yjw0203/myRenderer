#include "Engine/Animation/Public/Animation.h"

namespace yjw
{
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
}