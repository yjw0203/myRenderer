#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/Render/Private/Material.h"
#include "Engine/Animation/Public/AnimationData.h"
#include "Engine/Engine/Public/Asset/Material.h"
#include "Engine/Engine/Public/Asset/Mesh.h"
#include "Engine/Engine/Public/Asset/Animation.h"

namespace yjw
{
    class MaterialInstance;
    class Material;
    enum ModelFileFormat
    {
        model_file_format_pmx,
        model_file_format_gltf,
        model_file_format_glb,
    };

    class CPUModel
    {
        friend class ModelBuildImplement;
        friend class ModelBuilder;
    public:
        typedef int BufferViewId;
        struct CPUBuffer
        {
            MeshVertexType type;
            std::vector<char> buffer;
        };
        struct CPUBufferView
        {
            int buffer_id;
            int offset;
            int width;
        };
        struct VertexBuffer
        {
            MeshVertexType type;
            BufferViewId buffer;
        };
        struct CPUMesh
        {
            int first_index;
            int index_count;
            BufferViewId index_buffer;
            std::vector<VertexBuffer> vertex_buffers;
            bool is_indices_16bit{ false };
        };
        struct Texture
        {
            std::string texture_name;

            std::string m_imported_name;
        };
        struct Entity
        {
            int mesh_id;
            int material_ins_id;
        };

        struct Material
        {
            std::string m_shader;
            std::string m_entry;
        };

        struct MaterialIns
        {
            int material_id;
            std::map<std::string, float> m_float_params;
            std::map<std::string, glm::vec2> m_vec2_params;
            std::map<std::string, glm::vec3> m_vec3_params;
            std::map<std::string, glm::vec4> m_vec4_params;
            std::map<std::string, glm::mat4x4> m_mat4x4_params;
            std::map<std::string, int> m_texture_params;

            std::string m_imported_mat_name;
            std::map<std::string, std::string> m_decided_texture_params;
        };

    public:
        std::vector<CPUBuffer> m_pool_buffers;
        std::vector<Texture> m_pool_textures;
        std::vector<CPUBufferView> m_pool_buffer_views;
        std::vector<CPUMesh> m_meshes;
        std::vector<Material> m_material;
        std::vector<MaterialIns> m_material_instances;
        std::vector<Entity> m_entities;

        SkeletonAST m_skeleton_data;

        AnimationClipAST m_animation_clip_data;
    };

    class ModelBuilder;
    class Model
    {
        friend class ModelBuilder;
    public:
        Model() {};
        static std::unique_ptr<ModelBuilder> load(std::string filePath);
        
        std::shared_ptr<CPUModel> m_cpu_model;
    };

    class ModelBuilder
    {
    public:
        ModelBuilder() {};
        operator std::shared_ptr<Model>() const { return std::move(model); }
        void build(std::string filePath, std::string fileName, ModelFileFormat format);
    private:
        std::shared_ptr<Model> model;
    };

}