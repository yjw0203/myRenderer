#pragma once
#include <memory>
#include <vector>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Engine/Engine/Public/Asset/Material.h"
#include "Engine/Render/Private/RenderResource.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"
#include "projectInfo.h"

namespace yjw
{
    struct MaterialShader
    {
        std::string shader_path;
        std::string entry_name;
    };

    class MaterialParameterPool
    {
        friend class Material;
        friend class MaterialInstance;
    public:
        struct MaterialParameter
        {
            std::string name;
            int offset;
            int width;
        };
        struct UBOBinding
        {
            std::string name;
            int offset;
            int block_size;
            rpi::RHIShaderType shaderType;
        };

        void AddUBOLayout(rpi::RHIShaderType shaderType, rhi::ShaderReflect::UBO& ubo);
        void FlushLayoutSet();
        void Clear();

        void SetData(const std::string& name, void* data, int size);
        void SetDataFloat(const std::string& name, float value);
        void SetDataVec2(const std::string& name, glm::vec2 value);
        void SetDataVec3(const std::string& name, glm::vec3 value);
        void SetDataVec4(const std::string& name, glm::vec4 value);
        void SetDataMat4(const std::string& name, glm::mat4x4 value);
        void FlushCpuDataToGpu();

    private:
        std::vector<MaterialParameter> m_parameters;
        std::vector<UBOBinding> m_ubo_bindings;
        std::vector<rpi::RPIBuffer> m_gpu_views;
        int m_size = 0;
        void* m_data = nullptr;
        rpi::RPIBuffer m_gpu_data = rpi::RPIBuffer::Null;
        bool m_cpu_data_dirty = false;
    };

    class Material : public RenderResource
    {
        friend class MaterialInstance;
    public:
        Material();
        ~Material();
        rpi::RPIShader GetPixelShader();
        void Build(const char* shader, const char* entry);
        void Build(const char* url);
    private:
        void Destroy();
    private:
        MaterialShader m_ps{};
        rpi::RPIShader m_pixel_shader{};
        rhi::ShaderReflect* m_ps_reflect{};

        Asset<MaterialAST> m_asset{};
    };

    class MaterialInstance : public RenderResource
    {
    public:
        MaterialInstance();
        ~MaterialInstance();
        void Build(Material* material);
        void Build(const char* url);
        void Destroy();
        void SetDataFloat(const std::string& name, float value);
        void SetDataVec2(const std::string& name, glm::vec2 value);
        void SetDataVec3(const std::string& name, glm::vec3 value);
        void SetDataVec4(const std::string& name, glm::vec4 value);
        void SetDataMat4(const std::string& name, glm::mat4x4 value);
        void SetTexture(const std::string& name, rpi::RPITexture texture);
        void FlushDataToGpu();
        rpi::RPIResourceSet& GetResourceSet();
        rpi::RPIShader GetPixelShader();
    private:
        Material* m_material = nullptr;
        MaterialParameterPool m_parameters_pool;
        rpi::RPIResourceSet m_ps_resource_set{};

        Asset<MaterialInstanceAST> m_asset;
    };

    extern MaterialInstance* g_default_material_instance;
}