#include "yjw_gltf_model.h"
#include "Engine/Model/Private/3rd/gltf/tiny_gltf.h"
#include <memory>
#include <iostream>
#include "Engine/Engine/Public/Asset/Mesh.h"

namespace yjw
{
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
            return MeshVertexType::TARGENT;
        }
        if (attribute == "TEXCOORD_0")
        {
            return MeshVertexType::UV0;
        }
        return MeshVertexType::UNKNOW;
    }

    GLTFModelBuilder::GLTFModelBuilder(bool isBinary)
    {
        m_is_binary = isBinary;
    }

    void GLTFModelBuilder::FillModelData(const std::string& filePath, const std::string& fileName)
    {
        tinygltf::Model gltfModel;
        std::string holePath = filePath + "/" + fileName;
        LoadGLTFModel(gltfModel, holePath.c_str(), m_is_binary);
        for (tinygltf::Buffer& buffer : gltfModel.buffers)
        {
            AddBuffer(MeshVertexType::UNKNOW ,buffer.data.data(), buffer.data.size());
        }
        for (tinygltf::BufferView& bufferView : gltfModel.bufferViews)
        {
            AddBufferView(bufferView.buffer, bufferView.byteOffset, bufferView.byteLength);
        }
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
                    SetBufferType(GetBufferIDFromBufferView(gltfModel.accessors[attribute.second].bufferView), ConvertGLTFVertexAttribute(attribute.first));
                    AddVertexBuffer(mesh_id, ConvertGLTFVertexAttribute(attribute.first), gltfModel.accessors[attribute.second].bufferView);
                }
                tinygltf::Accessor& accessor = gltfModel.accessors[gltfPrimitive.indices];
                bool is16Bit = gltfModel.bufferViews[accessor.bufferView].byteLength / accessor.count == 2;
                AddIndexBuffer(mesh_id, accessor.bufferView, accessor.byteOffset / (is16Bit ? 2 : 4), accessor.count, is16Bit);
                AddEntity(mesh_id, gltfPrimitive.material);
            }
        }

        
    }
}