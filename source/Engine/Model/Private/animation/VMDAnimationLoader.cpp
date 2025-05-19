#include "Engine/Model/Public/AnimationLoader.h"
#include "Engine/Model/Private/3rd/saba/VMDFile.h"

namespace yjw
{
    void SetVMDBezier(Bezier& bezier, const unsigned char* cp)
    {
        int x0 = cp[0];
        int y0 = cp[4];
        int x1 = cp[8];
        int y1 = cp[12];

        bezier.m_cp1 = glm::vec2((float)x0 / 127.0f, (float)y0 / 127.0f);
        bezier.m_cp2 = glm::vec2((float)x1 / 127.0f, (float)y1 / 127.0f);
    }

    glm::mat3 InvZ(const glm::mat3& m)
    {
        const glm::mat3 invZ = glm::scale(glm::mat4(1), glm::vec3(1, 1, -1));
        return invZ * m * invZ;
    }

    void VMD::Load(const std::string& url)
    {
        saba::VMDFile vmd;
        ReadVMDFile(&vmd, url.c_str());

        m_bezier_animation_key = std::make_shared<BezierAnimationKey>();

        for (auto& motion : vmd.m_motions)
        {
            BezierAnimationKey::KeyNode node;
            node.m_name = motion.m_boneName.ToUtf8String();

            node.m_time = int32_t(motion.m_frame);

            node.m_translate = motion.m_translate;

            const glm::quat q = motion.m_quaternion;
            auto rot0 = glm::mat3_cast(q);
            auto rot1 = rot0;
            node.m_rotate = glm::quat_cast(rot1);

            SetVMDBezier(node.m_txBezier, &motion.m_interpolation[0]);
            SetVMDBezier(node.m_tyBezier, &motion.m_interpolation[1]);
            SetVMDBezier(node.m_tzBezier, &motion.m_interpolation[2]);
            SetVMDBezier(node.m_rotBezier, &motion.m_interpolation[3]);

            if (!m_bezier_animation_key->m_bone_index_map.count(node.m_name))
            {
                m_bezier_animation_key->m_bone_index_map[node.m_name] = m_bezier_animation_key->m_nodes.size();
                m_bezier_animation_key->m_nodes.push_back(std::vector<BezierAnimationKey::KeyNode>());
            }
            m_bezier_animation_key->m_nodes[m_bezier_animation_key->m_bone_index_map[node.m_name]].push_back(node);
        }

        ConvertBezierAnimationKeyToAnimationClip(m_bezier_animation_key, m_animation_clip);
    }

    void GetTransformFromBezierAnimation(float play_time, const std::vector<BezierAnimationKey::KeyNode>& nodes, Vector3& out_pos, Quaternion& out_quat)
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

        out_pos = vt;
        out_quat = q;
    }

    void VMD::ConvertBezierAnimationKeyToAnimationClip(std::shared_ptr<BezierAnimationKey> bezier_animation_key, AnimationClipAST& animation_clip)
    {
        if (!bezier_animation_key)
        {
            return;
        }
        animation_clip.m_time_per_frame = 0.33;
        float max_time = 0.f;
        for (std::vector<BezierAnimationKey::KeyNode>& channel : bezier_animation_key->m_nodes)
        {
            if (channel.size() == 0)
            {
                continue;
            }
            animation_clip.m_animation_channels.push_back(AnimationChannel{});
            animation_clip.m_animation_channels.back().m_name = channel[0].m_name;
            for (BezierAnimationKey::KeyNode& node : channel)
            {
                max_time = std::max(max_time, (float)node.m_time);
            }
        }
        animation_clip.m_total_frame = (int)(max_time / animation_clip.m_time_per_frame) + 1;

        int channel_index = 0;
        for (std::vector<BezierAnimationKey::KeyNode>& channel : bezier_animation_key->m_nodes)
        {
            if (channel.size() == 0)
            {
                continue;
            }
            animation_clip.m_animation_channels[channel_index].m_position_keys.resize(animation_clip.m_total_frame);
            animation_clip.m_animation_channels[channel_index].m_rotation_keys.resize(animation_clip.m_total_frame);
            animation_clip.m_animation_channels[channel_index].m_scaling_keys.resize(animation_clip.m_total_frame);
            for (int frame_index = 0; frame_index < animation_clip.m_total_frame; frame_index++)
            {
                float time = frame_index * animation_clip.m_time_per_frame;
                Vector3 position;
                Quaternion rotation;
                GetTransformFromBezierAnimation(time, channel, position, rotation);
                animation_clip.m_animation_channels[channel_index].m_position_keys[frame_index] = position;
                animation_clip.m_animation_channels[channel_index].m_rotation_keys[frame_index] = rotation;
                animation_clip.m_animation_channels[channel_index].m_scaling_keys[frame_index] = Vector3(1, 1, 1);
            }
            channel_index++;
        }
    }
}