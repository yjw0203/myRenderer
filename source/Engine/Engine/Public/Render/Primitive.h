#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/Engine/Public/Asset/Mesh.h"
#include "Engine/Asset/Public/Asset.h"

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
        Primitive() {}
        ~Primitive();
        void BuildGpuPrimitive();
        rpi::RPIBuffer GetVertexBuffer(VertexType type);
        rpi::RPIBuffer GetIndexBuffer();
        rpi::RPIPrimitiveBinding GetPrimitiveBinding();
        rpi::RPIShader GetVertexShader();
        rpi::RPIResourceSet GetVSResourceSet();
    protected:
        struct SubMesh
        {
            uint32_t offset;
            uint32_t size;
        };

        Asset<MeshAST> m_mesh_ast;

        rpi::RPIBuffer m_vertex_buffers[(int)VertexType::count];
        rpi::RPIBuffer m_index_buffer;
        bool m_is_indices_16bit = false;
        //todo : sub mesh
        rpi::RPIPrimitiveBinding m_primitive_binding{};

        rpi::RPIResourceSet m_vs_resource_set{};
        rpi::RPIShader m_vertex_shader;
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