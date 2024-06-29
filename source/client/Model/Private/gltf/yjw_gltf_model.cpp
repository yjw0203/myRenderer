#include "yjw_gltf_model.h"
#include "tiny_gltf.h"
#include <memory>
#include <iostream>

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

    VertexAttributeType ConvertGLTFVertexAttribute(const std::string& attribute)
    {
        if (attribute == "POSITION")
        {
            return VertexAttributeType::position;
        }
        if (attribute == "NORMAL")
        {
            return VertexAttributeType::normal;
        }
        if (attribute == "TANGENT")
        {
            return VertexAttributeType::tangent;
        }
        if (attribute == "TEXCOORD_0")
        {
            return VertexAttributeType::uv0;
        }
        return VertexAttributeType::unkown;
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
            AddBuffer(buffer.data.data(), buffer.data.size());
        }
        for (tinygltf::BufferView& bufferView : gltfModel.bufferViews)
        {
            AddBufferView(bufferView.buffer, bufferView.byteOffset, bufferView.byteLength);
        }
        for (tinygltf::Image& gltfImage : gltfModel.images)
        {
            AddTexture(gltfImage.name, gltfImage.width, gltfImage.height, gltfImage.image.data(), gltfImage.image.size());
        }
        for (tinygltf::Material& gmaterial : gltfModel.materials)
        {
            MaterialInstance* material = new MaterialInstance(&g_pbr_material);
            if (gmaterial.pbrMetallicRoughness.baseColorTexture.index != -1)
            {
                material->SetTexture("albedoTex", GetTexture(gltfModel.textures[gmaterial.pbrMetallicRoughness.baseColorTexture.index].source));
            }
            else
            {
                material->SetTexture("albedoTex", GetTexture(0));
            }
            material->SetDataVec2("metallic_roughness", glm::vec2(gmaterial.pbrMetallicRoughness.metallicFactor, gmaterial.pbrMetallicRoughness.roughnessFactor));
            int material_id = AddMaterial(material);
        }
        for (tinygltf::Mesh& gltfMesh : gltfModel.meshes)
        {
            for (tinygltf::Primitive& gltfPrimitive : gltfMesh.primitives)
            {
                int mesh_id = AddMesh();
                for (std::pair<const std::string, int>& attribute : gltfPrimitive.attributes)
                {
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