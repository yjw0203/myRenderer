#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/Engine/Public/Asset/Mesh.h"
#include "Engine/Asset/Public/Asset.h"
#include "Engine/Render/Private/RenderResource.h"
#include "Engine/Render/Private/Material.h"

namespace yjw
{
    struct SubPrimitive
    {
        int m_sub_primitive_id{};
        MaterialInstance* m_material{};
        rpi::RPIResourceBinding m_resource_binding{};
        rpi::RPIPipeline m_pipeline{};
    };

    class Primitive : public RenderResource
    {
    public:
        Primitive() {}
        Primitive(const char* mesh) { m_mesh_ast.SetURL(mesh); }
        ~Primitive();
        void BuildGpuPrimitive();
        rpi::RPIPrimitiveBinding GetPrimitiveBinding();
        rpi::RPIShader GetVertexShader();
        rpi::RPIResourceSet GetVSResourceSet();
        const std::vector<SubPrimitive>& GetSubPrimitive();

    protected:
        Asset<MeshAST> m_mesh_ast;
        
        std::vector<SubPrimitive> m_sub_primitives;
        std::vector<Material*> m_materials;
        std::vector<MaterialInstance*> m_material_instances;

        // rhi resource
        struct VB
        {
            MeshVertexType m_type;
            rpi::RPIBuffer m_buffer;
        };
        struct IB
        {
            rpi::RPIBuffer m_buffer;
            bool m_is_indices_16bit = false;
        };
        std::vector<VB> m_vertex_buffers;
        IB m_index_buffer;

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