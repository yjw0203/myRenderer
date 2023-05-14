#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "rhi/rhi/yjw_rhi_header.h"

namespace yjw
{
    struct MeshVertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    struct Material
    {
        uint32_t offset;
        uint32_t size;
        rhi::RHITexture2DFromFile* texture;
        rhi::RHIResourceView* textureView;
    };

    class MeshBuilder;
    class Mesh
    {
        friend class MeshBuilder;
    public:
        Mesh() {};
        static std::unique_ptr<MeshBuilder> load(std::string filePath, std::string fileName);
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Material> materials;
        
    };

    class MeshBuilder
    {
    public:
        MeshBuilder() {};
        operator Mesh() const { return std::move(mesh); }
        void build(std::string filePath, std::string fileName);
    private:
        Mesh mesh;
    };

}