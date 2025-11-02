#include "Engine/Animation/Public/Skeleton.h"
#include "Engine/Utils/Public/Algorithm/Algorithm.h"
#include <unordered_map>

Skeleton::~Skeleton()
{

}

bool Skeleton::BuildSkeleton(const char* url)
{
    Asset<SkeletonAST> ast;
    ast.SetURL(url);
    return BuildSkeleton(ast.GetData()->m_bones_map, ast.GetData()->m_skin_map);
}

bool Skeleton::BuildSkeleton(const std::vector<RawBone>& m_bones_map, const std::vector<int>& skin_map)
{
    TopologicalSort TopoSort(m_bones_map.size());
    for (auto& bone : m_bones_map)
    {
        if (bone.m_parent_index != bone.m_parent_index)
        {
            TopoSort.AddEdge(bone.m_parent_index, bone.m_index);
        }
    }
    if (!TopoSort.Sort())
    {
        return false;
    }

    std::unordered_map<int, int> index_map;
    m_bones.resize(m_bones_map.size());
    for (int index = 0; index < m_bones.size(); index++)
    {
        const RawBone& bone_data = m_bones_map[TopoSort.GetResult()[index]];
        index_map[bone_data.m_index] = index;
        m_bones[index].m_index = bone_data.m_index;
        m_bones[index].m_name = bone_data.m_name;
        m_bones[index].m_init_pose = bone_data.m_binding_pose;
        m_bones[index].m_inverse_init = bone_data.m_inverse_T_matrix;
        if (index_map.count(bone_data.m_parent_index))
        {
            int parent_index = index_map[bone_data.m_parent_index];
            m_bones[index].m_parent = &m_bones[parent_index];
        }
    }

    m_skin_map.resize(skin_map.size());
    for (int i = 0; i < skin_map.size(); i++)
    {
        m_skin_map[i] = TopoSort.GetRResult()[skin_map[i]];
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

Bone::List* Skeleton::GetBones()
{
    return &m_bones;
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
        //m_bones[index].m_pose.m_rotate = m_bones[index].m_pose.m_rotate * m_bones[index].m_anim_pose.m_rotate;
        m_bones[index].m_pose.m_rotate = m_bones[index].m_anim_pose.m_rotate;
        m_bones[index].m_pose.m_rotate = glm::normalize(m_bones[index].m_pose.m_rotate);

        m_bones[index].m_pose.m_scale = m_bones[index].m_pose.m_scale * m_bones[index].m_anim_pose.m_scale;

        //m_bones[index].m_pose.m_location += m_bones[index].m_anim_pose.m_location;
        m_bones[index].m_pose.m_location = m_bones[index].m_anim_pose.m_location;

        m_bones[index].m_local = m_bones[index].m_pose.getMatrix();
    }

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
        //m_bone_matrices[m_bones[index].m_index] = glm::transpose(m_bone_matrices[m_bones[index].m_index]);
        //m_bone_matrices[m_bones[index].m_index] = parent->m_inverse_init * parent->m_local;
        //m_bone_matrices[m_bones[index].m_index] = m_bones[index].m_inverse_init * m_bones[index].m_init_pose.getMatrix();
    }

    m_bone_matrices.resize(m_skin_map.size());
    for (int i = 0; i < m_bone_matrices.size(); i++)
    {
        int bone_index = m_skin_map[i];
        m_bone_matrices[i] = m_bones[bone_index].m_local * m_bones[bone_index].m_inverse_init;
    }
}
