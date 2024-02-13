#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "rhi/rpi/yjw_rpi_header.h"

namespace yjw
{
    struct MeshVertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
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
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;

        rpi::RPIBuffer vertex_buffer;
        rpi::RPIBuffer index_buffer;
    };

    class Material
    {
    public:
        rpi::RPITexture texture;
        rpi::RPIDescriptor textureShaderResource;
        glm::vec4    diffuse;
        glm::vec3    specular;
        float        specularPower;
        glm::vec3    ambient;
    };


    struct Entity
    {
        std::shared_ptr<Mesh> mesh;
        uint32_t subMeshId;
        std::shared_ptr<Material> material;
    };

    class ModelBuilder;
    class Model
    {
        friend class ModelBuilder;
    public:
        Model() {};
        static std::unique_ptr<ModelBuilder> load(std::string filePath, std::string fileName);
        
        
        std::shared_ptr<Mesh> mesh;
        std::vector<std::shared_ptr<Material> > materials;
        std::vector<Entity> entitys;
    };

    class ModelBuilder
    {
    public:
        ModelBuilder() {};
        operator std::shared_ptr<Model>() const { return std::move(model); }
        void build(std::string filePath, std::string fileName);
    private:
        std::shared_ptr<Model> model;
    };

}