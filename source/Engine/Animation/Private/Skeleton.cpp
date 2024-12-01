#include "Engine/Animation/Public/Skeleton.h"
#include "Engine/Utils/Public/Algorithm/Algorithm.h"
#include <unordered_map>

namespace yjw
{
    Skeleton::~Skeleton()
    {

    }

    bool Skeleton::BuildSkeleton(const SkeletonData& skeleton_data)
    {
        TopologicalSort TopoSort(skeleton_data.bones.size());
        for (auto& bone : skeleton_data.bones)
        {
            TopoSort.AddEdge(bone.parent_index, bone.index);
        }
        if (!TopoSort.Sort())
        {
            return false;
        }

        std::unordered_map<int, int> index_map;
        m_bones.resize(skeleton_data.bones.size());
        for (int index = 0; index < m_bones.size(); index++)
        {
            const BoneData& bone_data = skeleton_data.bones[TopoSort.GetResult()[index]];
            index_map[bone_data.index] = index;
            m_bones[index].m_index = bone_data.index;
            m_bones[index].m_name = bone_data.name;
            m_bones[index].m_init_pose = bone_data.binding_pose;
            m_bones[index].m_inverse_init = glm::inverse(m_bones[index].m_init_pose.getMatrix());
            if (index_map.count(bone_data.parent_index))
            {
                int parent_index = index_map[bone_data.parent_index];
                m_bones[index].m_parent = &m_bones[parent_index];
                m_bones[index].m_inverse_init = m_bones[index].m_inverse_init * m_bones[index].m_parent->m_inverse_init;
            }
        }

        return true;
    }

    void Skeleton::LoadController(std::shared_ptr<SkeletonController> controller)
    {
        m_controller = controller;
        m_controller->Bind(&m_bones);
    }

    void Skeleton::Update(float time)
    {
        BeginUpdateAnimation();
        if (m_controller)
        {
            m_controller->Evaluate(time, 1.0f);
        }
        EndUpdateAnimation();
    }

    const Bone::List& Skeleton::GetBones()
    {
        return m_bones;
    }

    const std::vector<Matrix4x4>& Skeleton::GetBoneMatrices()
    {
        return m_bone_matrices;
    }

    void Skeleton::BeginUpdateAnimation()
    {
        for (int index = 0; index < m_bones.size(); index++)
        {
            m_bones[index].m_pose = m_bones[index].m_init_pose;
            m_bones[index].m_anim_pose = Transform();
        }
    }

    void Skeleton::EndUpdateAnimation()
    {
        for (int index = 0; index < m_bones.size(); index++)
        {
            m_bones[index].m_pose.m_rotate = m_bones[index].m_pose.m_rotate * m_bones[index].m_anim_pose.m_rotate;
            m_bones[index].m_pose.m_rotate = glm::normalize(m_bones[index].m_pose.m_rotate);

            m_bones[index].m_pose.m_scale = m_bones[index].m_pose.m_scale * m_bones[index].m_anim_pose.m_scale;

            m_bones[index].m_pose.m_location += m_bones[index].m_anim_pose.m_location;

            m_bones[index].m_local = m_bones[index].m_pose.getMatrix();
        }

        m_bone_matrices.resize(m_bones.size());
        for (int index = 0; index < m_bones.size(); index++)
        {
            Bone* parent = m_bones[index].m_parent;
            if (parent)
            {
                /*
                m_bones[index].m_pose.m_location = m_bones[index].m_pose.m_location + parent->m_pose.m_location * m_bones[index].m_pose.m_scale * m_bones[index].m_pose.m_rotate;
                
                m_bones[index].m_pose.m_rotate = m_bones[index].m_pose.m_rotate * parent->m_pose.m_rotate;
                m_bones[index].m_pose.m_rotate = glm::normalize(m_bones[index].m_pose.m_rotate);

                m_bones[index].m_pose.m_scale = m_bones[index].m_pose.m_scale * parent->m_pose.m_scale;
                */
                m_bones[index].m_local = parent->m_local * m_bones[index].m_local;

            }
            //m_bone_matrices[m_bones[index].m_index] = m_bones[index].m_inverse_init * m_bones[index].m_pose.getMatrix();
            m_bone_matrices[m_bones[index].m_index] = m_bones[index].m_local * m_bones[index].m_inverse_init;
            //m_bone_matrices[m_bones[index].m_index] = glm::transpose(m_bone_matrices[m_bones[index].m_index]);
            //m_bone_matrices[m_bones[index].m_index] = parent->m_inverse_init * parent->m_local;
            //m_bone_matrices[m_bones[index].m_index] = m_bones[index].m_inverse_init * m_bones[index].m_init_pose.getMatrix();
        }
    }
}
