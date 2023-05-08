#pragma once
#include <string>
#include <memory>
#include <vector>

namespace yjw
{
    struct MeshVertex {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    class MeshBuilder;
    class Mesh
    {
        friend class MeshBuilder;
    public:
        Mesh() {};
        static std::unique_ptr<MeshBuilder> load(std::string filePath);
        std::vector<MeshVertex> vertices;
        std::vector<int> indices;
    };

    class MeshBuilder
    {
    public:
        MeshBuilder() {};
        operator Mesh() const { return std::move(mesh); }
        void build(std::string filePath);
    private:
        Mesh mesh;
    };

}