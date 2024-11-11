#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "RHI/rpi/yjw_rpi.h"

namespace yjw
{
    enum class VertexType : char
    {
        postion,
        normal,
        uv,
        count
    };

    class Primitive
    {
    public:
        void BuildGpuPrimitive();
        rpi::RPIBuffer GetVertexBuffer(VertexType type);
        rpi::RPIBuffer GetIndexBuffer();
        rpi::RPIPrimitiveBinding GetPrimitiveBinding();
    protected:
        struct SubMesh
        {
            uint32_t offset;
            uint32_t size;
        };

        std::vector<SubMesh> m_sub_meshes;
        std::vector<char> m_vertexes[(int)VertexType::count];
        std::vector<char> m_indices;

        rpi::RPIBuffer m_vertex_buffers[(int)VertexType::count];
        rpi::RPIBuffer m_index_buffer;
        bool m_is_indices_16bit = false;
        //todo : sub mesh
        rpi::RPIPrimitiveBinding m_primitive_binding{};

        //todo : remove
        rpi::RPIShader vs;
    };

    class BoxPrimitive : public Primitive
    {
    public:
        BoxPrimitive();
    };

    class SpherePrimitive : public Primitive
    {
    public:
        SpherePrimitive();
    };

    extern BoxPrimitive g_box_primitive;
}