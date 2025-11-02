#include "yjw_gltf_model.h"
#include "Engine/Model/Private/3rd/gltf/tiny_gltf.h"
#include <memory>
#include <iostream>
#include "Engine/Engine/Public/Asset/Mesh.h"

bool LoadGLTFModel(tinygltf::Model& model, const char* filename,bool isBinary)
{
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res;
    if (isBinary)
    {
        res = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    }
    else
    {
        res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }
        
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << filename << std::endl;
    else
        std::cout << "Loaded glTF: " << filename << std::endl;

    return res;
}

MeshVertexType ConvertGLTFVertexAttribute(const std::string& attribute)
{
    if (attribute == "POSITION")
    {
        return MeshVertexType::POSITION;
    }
    if (attribute == "NORMAL")
    {
        return MeshVertexType::NORMAL;
    }
    if (attribute == "TANGENT")
    {
        return MeshVertexType::TANGENT;
    }
    if (attribute == "TEXCOORD_0")
    {
        return MeshVertexType::UV0;
    }
    if (attribute == "TEXCOORD_1")
    {
        return MeshVertexType::UV1;
    }
    if (attribute == "JOINTS_0")
    {
        return MeshVertexType::BLEND_INDICES;
    }
    if (attribute == "WEIGHTS_0")
    {
        return MeshVertexType::BLEND_WEIGHTS;
    }
    if (attribute == "JOINTS_1")
    {
        return MeshVertexType::BLEND_INDICES1;
    }
    if (attribute == "WEIGHTS_1")
    {
        return MeshVertexType::BLEND_WEIGHTS1;
    }
    return MeshVertexType::UNKNOW;
}

GLTFModelBuilder::GLTFModelBuilder(bool isBinary)
{
    m_is_binary = isBinary;
}

template<typename T1, typename T2>
void ChangeBufferDataType(void* data, int byte_width, std::vector<T2>& buffer)
{
    buffer.resize(0);
    for (int i = 0; i < byte_width; i += sizeof(T1))
    {
        buffer.push_back((T2)(T1)((uint8_t*)data + i));
    }
}

template<typename T>
void GetAccessorData(tinygltf::Model& model, int accessor_id, std::vector<T>& out)
{
    tinygltf::Accessor& accessor = model.accessors[accessor_id];
    tinygltf::BufferView& buffer_view = model.bufferViews[accessor.bufferView];
    tinygltf::Buffer& buffer = model.buffers[buffer_view.buffer];

    void* data_ptr = buffer.data.data() + buffer_view.byteOffset + accessor.byteOffset;
        
    out.resize(accessor.count);
    for (int i = 0; i < accessor.count; i++)
    {
        out[i] = *((T*)data_ptr + i);
    }
}

void GLTFModelBuilder::FillModelData(const std::string& filePath, const std::string& fileName)
{
    tinygltf::Model gltfModel;
    std::string holePath = filePath + "/" + fileName;
    LoadGLTFModel(gltfModel, holePath.c_str(), m_is_binary);

    for (tinygltf::Image& gltfImage : gltfModel.images)
    {
        AddTexture(gltfImage.name, gltfImage.width, gltfImage.height, gltfImage.image.data(), gltfImage.image.size());
    }

    CPUModel::Material material_ast{};
    material_ast.m_shader = "ForwardPBR.hlsl";
    material_ast.m_entry = "PSMain";
    int mat_id = AddMaterial(material_ast);

    for (tinygltf::Material& gmaterial : gltfModel.materials)
    {
        CPUModel::MaterialIns mat_ins{};
        mat_ins.material_id = mat_id;
        if (gmaterial.pbrMetallicRoughness.baseColorTexture.index != -1)
        {
            mat_ins.m_texture_params["albedoTex"] = gltfModel.textures[gmaterial.pbrMetallicRoughness.baseColorTexture.index].source;
        }
        else
        {
            mat_ins.m_texture_params["albedoTex"] = 0;
        }
        mat_ins.m_float_params["metallic"] = gmaterial.pbrMetallicRoughness.metallicFactor;
        mat_ins.m_float_params["roughness"] = gmaterial.pbrMetallicRoughness.roughnessFactor;
        int mat_ins_id = AddMaterialInstance(mat_ins);
    }
    for (tinygltf::Mesh& gltfMesh : gltfModel.meshes)
    {
        for (tinygltf::Primitive& gltfPrimitive : gltfMesh.primitives)
        {
            int mesh_id = AddMesh();
            for (std::pair<const std::string, int>& attribute : gltfPrimitive.attributes)
            {
                tinygltf::Accessor& accessor = gltfModel.accessors[attribute.second];
                MeshVertexType vertex_type = ConvertGLTFVertexAttribute(attribute.first);
                int bufferView = gltfModel.accessors[attribute.second].bufferView;
                void* data_ptr = (uint8_t*)gltfModel.buffers[gltfModel.bufferViews[bufferView].buffer].data.data() + gltfModel.bufferViews[bufferView].byteOffset;
                int byte_width = gltfModel.bufferViews[bufferView].byteLength;

                int buffer_id = AddBuffer(vertex_type, data_ptr, byte_width);
                AddVertexBuffer(mesh_id, vertex_type, AddBufferView(buffer_id));
            }
            tinygltf::Accessor& accessor = gltfModel.accessors[gltfPrimitive.indices];
            bool is16Bit = gltfModel.bufferViews[accessor.bufferView].byteLength / accessor.count == 2;
            int buffer_id = AddBuffer(MeshVertexType::INDEX, (uint8_t*)gltfModel.buffers[gltfModel.bufferViews[accessor.bufferView].buffer].data.data() + gltfModel.bufferViews[accessor.bufferView].byteOffset, gltfModel.bufferViews[accessor.bufferView].byteLength);
            AddIndexBuffer(mesh_id, AddBufferView(buffer_id), accessor.byteOffset / (is16Bit ? 2 : 4), accessor.count, is16Bit);
            AddEntity(mesh_id, gltfPrimitive.material);
        }
    }

    GetSkeleton().m_bones_map.resize(gltfModel.nodes.size());
    for (int i = 0; i < gltfModel.nodes.size(); i++)
    {
        tinygltf::Node& node = gltfModel.nodes[i];
        GetSkeleton().m_bones_map[i].m_name = node.name;
        if (node.translation.size() == 3)
        {
            GetSkeleton().m_bones_map[i].m_binding_pose.m_location = Vector3(node.translation[0], node.translation[1], node.translation[2]);
        }
        if (node.rotation.size() == 4)
        {
            GetSkeleton().m_bones_map[i].m_binding_pose.m_rotate = Quaternion(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
        }
        GetSkeleton().m_bones_map[i].m_binding_pose.m_scale = Vector3(1, 1, 1);
        GetSkeleton().m_bones_map[i].m_index = i;

        for (int child : node.children)
        {
            GetSkeleton().m_bones_map[child].m_parent_index = i;
        }
    }

    if (gltfModel.skins.size() == 1)
    {
        GetSkeleton().m_skin_map = gltfModel.skins[0].joints;
        std::vector<glm::mat4x4> inverse_t_mat;
        GetAccessorData<glm::mat4x4>(gltfModel, gltfModel.skins[0].inverseBindMatrices, inverse_t_mat);
        for (int i = 0; i < GetSkeleton().m_skin_map.size(); i++)
        {
            int bone_index = GetSkeleton().m_skin_map[i];
            GetSkeleton().m_bones_map[bone_index].m_inverse_T_matrix = inverse_t_mat[i];
        }
    }

    if (gltfModel.animations.size() == 1)
    {
        GetAnimationClip().m_time_per_frame = 1.0 / 30;
        GetAnimationClip().m_animation_channels.resize(gltfModel.animations[0].channels.size() / 3);
        for (int i = 0; i < GetAnimationClip().m_animation_channels.size(); i++)
        {
            AnimationChannel& channel = GetAnimationClip().m_animation_channels[i];
            channel.m_name = gltfModel.nodes[gltfModel.animations[0].channels[i * 3].target_node].name;
            for (int j = 0; j < 3; j++)
            {
                tinygltf::AnimationChannel& gtf_channel = gltfModel.animations[0].channels[i * 3 + j];
                tinygltf::AnimationSampler& gtf_sampler = gltfModel.animations[0].samplers[gtf_channel.sampler];

                std::vector<float> time_line;
                GetAccessorData<float>(gltfModel, gtf_sampler.input, time_line);

                if (gtf_channel.target_path == "translation")
                {
                    std::vector<glm::vec3> line;
                    GetAccessorData<glm::vec3>(gltfModel, gtf_sampler.output, line);
                    channel.m_position_keys = line;
                }
                else if (gtf_channel.target_path == "rotation")
                {
                    struct Tmp
                    {
                        float x;
                        float y;
                        float z;
                        float w;
                    };
                    std::vector<Tmp> line;
                    GetAccessorData<Tmp>(gltfModel, gtf_sampler.output, line);
                    std::vector<glm::quat> line_q;
                    line_q.resize(line.size());
                    for (int k = 0; k < line.size(); k++)
                    {
                        line_q[k] = glm::quat(line[k].w, line[k].x, line[k].y, line[k].z);
                    }
                    channel.m_rotation_keys = line_q;
                }
                else if (gtf_channel.target_path == "scale")
                {
                    std::vector<glm::vec3> line;
                    GetAccessorData<glm::vec3>(gltfModel, gtf_sampler.output, line);
                    channel.m_scaling_keys = line;
                }

            }
        }
        GetAnimationClip().m_total_frame = GetAnimationClip().m_animation_channels[0].m_position_keys.size();
    }
}
