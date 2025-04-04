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

    void VMD::Load(std::string filePath, std::string fileName)
    {
        std::string url = filePath + "/" + fileName;
        saba::VMDFile vmd;
        ReadVMDFile(&vmd, url.c_str());

        std::shared_ptr<BezierAnimationKey> key = std::make_shared<BezierAnimationKey>();

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

            if (!key->m_bone_index_map.count(node.m_name))
            {
                key->m_bone_index_map[node.m_name] = key->m_nodes.size();
                key->m_nodes.push_back(std::vector<BezierAnimationKey::KeyNode>());
            }
            key->m_nodes[key->m_bone_index_map[node.m_name]].push_back(node);
        }

        m_animation = std::make_shared<BezierAnimation>(key);
    }
}