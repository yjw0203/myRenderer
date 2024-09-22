#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "RHI/rpi/yjw_rpi_header.h"
#include "Model/yjw_material.h"
#include "Animation/AnimationData.h"

namespace yjw
{
    struct MeshVertex {
        std::vector<glm::vec3> pos;
        std::vector<glm::vec3> normal;
        std::vector<glm::vec2> uv;
    };

    struct MeshVertexBuffer {
        rpi::RPIBuffer pos;
        rpi::RPIBuffer normal;
        rpi::RPIBuffer uv;
    };

    class Mesh
    {
    public:
        struct SubMesh
        {
            uint32_t offset;
            uint32_t size;
        };

        std::vector<SubMesh> subMeshes;
        MeshVertex vertices;
        std::vector<uint32_t> indices;

        MeshVertexBuffer vertex_buffer;
        rpi::RPIBuffer index_buffer;
    };

    enum ModelFileFormat
    {
        model_file_format_pmx,
        model_file_format_gltf,
        model_file_format_glb,
    };

    enum VertexAttributeType
    {
        unkown,
        position,
        normal,
        tangent,
        uv0,
        blend_indices,
        blend_weights,
        blend_type
    };

    rhi::RHIName CastToRHIName(VertexAttributeType type);

    class CPUModel
    {
        friend class ModelBuildImplement;
        friend class ModelBuilder;
    public:
        typedef int BufferViewId;
        struct CPUBuffer
        {
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
            VertexAttributeType type;
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
            rpi::RPITexture texture;
        };
        struct Entity
        {
            int mesh_id;
            int material_id;
        };
    private:
        std::vector<CPUBuffer> m_pool_buffers;
        std::vector<Texture> m_pool_textures;
        std::vector<CPUBufferView> m_pool_buffer_views;
        std::vector<CPUMesh> m_meshes;
        std::vector<MaterialInstance*> m_material;
        std::vector<Entity> m_entities;

    public:
        SkeletonData m_skeleton_data;
    };

    class GPUModel
    {
        friend class ModelBuilder;
    public:
        typedef int BufferViewId;
        struct VertexBuffer
        {
            VertexAttributeType type;
            rpi::RPIBuffer buffer;
        };
        struct GPUMesh
        {
            int first_index;
            int index_count;
            rpi::RPIBuffer index_buffer;
            std::vector<VertexBuffer> vertex_buffers;
            bool is_indices_16bit{ false };
        };
        struct Texture
        {
            std::string texture_name;
            rpi::RPITexture texture;
        };
        struct Entity
        {
            int mesh_id;
            int material_id;
        };
    public:
        ~GPUModel();
        int GetEntityCount();
        GPUMesh* GetGPUMesh(int entity_id);
        MaterialInstance* GetMaterial(int entity_id);
        
    private:
        std::vector<rpi::RPIBuffer> m_pool_buffers;
        std::vector<rpi::RPIBuffer> m_pool_buffer_views;
        std::vector<Texture> m_pool_textures;
        std::vector<GPUMesh> m_meshes;
        std::vector<MaterialInstance*> m_material;
        std::vector<Entity> m_entities;
    };

    class ModelBuilder;
    class Model
    {
        friend class ModelBuilder;
    public:
        Model() {};
        static std::unique_ptr<ModelBuilder> load(std::string filePath, std::string fileName, ModelFileFormat format);
        
        std::shared_ptr<CPUModel> m_cpu_model;
        std::shared_ptr<GPUModel> m_gpu_model;
    };

    class ModelBuilder
    {
    public:
        ModelBuilder() {};
        operator std::shared_ptr<Model>() const { return std::move(model); }
        void build(std::string filePath, std::string fileName, ModelFileFormat format);
    private:
        std::shared_ptr<GPUModel> GenerateCPUModelToGPUModel(std::shared_ptr<CPUModel> cpu_model);
        std::shared_ptr<Model> model;
    };

}