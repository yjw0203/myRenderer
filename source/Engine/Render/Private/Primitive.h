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
        RdMaterial* m_material{};
        std::string m_material_slot{};
    };

    class RdGeometry : public RenderResource
    {
    public:
        RdGeometry() {}
        ~RdGeometry();
        void Build(const char* url);
        rpi::RPIPrimitiveBinding GetPrimitiveBinding();
        rpi::RPIShader GetVertexShader();
        rpi::RPIResourceSet GetVSResourceSet();
        const std::vector<SubPrimitive>& GetSubPrimitive();

    protected:
        void BuildGpuPrimitive();
        Asset<MeshAST> m_mesh_ast;
        
        std::vector<SubPrimitive> m_sub_primitives;
        std::vector<RdMaterialTemplate*> m_materials;
        std::vector<RdMaterial*> m_material_instances;

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

    class BoxPrimitive : public RdGeometry
    {
    public:
        BoxPrimitive();
    };

    class SpherePrimitive : public RdGeometry
    {
    public:
        SpherePrimitive();
    };

    extern BoxPrimitive g_box_primitive;
}