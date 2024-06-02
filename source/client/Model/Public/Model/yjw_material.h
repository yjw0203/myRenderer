#pragma once
#include <memory>
#include <vector>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "RHI/rpi/yjw_rpi_header.h"
#include "RHI/shaderCompiler/yjw_shader_compiler.h"
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

    class Material
    {
        friend class MaterialInstance;
    public:
        Material(const char* vs, const char* vs_entry, const char* ps, const char* ps_entry);
        ~Material();
        rpi::RPIPipeline GetPipeline();
    private:
        void BuildPipeline();
    private:
        MaterialShader m_vs{};
        MaterialShader m_ps{};
        rhi::ShaderReflect* m_vs_reflect{};
        rhi::ShaderReflect* m_ps_reflect{};
        rpi::RPIPipeline m_pipeline{};
        bool m_pipeline_builded = false;
    };

    class MaterialInstance
    {
    public:
        MaterialInstance(Material* material);
        ~MaterialInstance();
        void SetDataFloat(const std::string& name, float value);
        void SetDataVec2(const std::string& name, glm::vec2 value);
        void SetDataVec3(const std::string& name, glm::vec3 value);
        void SetDataVec4(const std::string& name, glm::vec4 value);
        void SetDataMat4(const std::string& name, glm::mat4x4 value);
        void SetTexture(const std::string& name, rpi::RPITexture texture);
        rpi::RPIResourceBinding& GetResourceBinding();
    private:
        Material* m_material = nullptr;
        MaterialParameterPool m_parameters_pool;
        rpi::RPIResourceBinding m_resource_binding{};
    };

    extern Material g_pbr_material;
}