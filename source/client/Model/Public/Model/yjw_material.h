#pragma once
#include <memory>
#include <vector>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "RHI/rpi/yjw_rpi_header.h"

namespace yjw
{
    struct MaterialShader
    {
        std::string shader_path;
        std::string entry_name;
    };

    struct MaterialParameter
    {
        std::string name;
        int offset;
        int width;
    };

    class MaterialParameterPool
    {
    public:
        void AddParameter(const std::string& name, int width);
    private:
        std::vector<MaterialParameter> m_parameters;
        int m_size = 0;
        void* m_data = nullptr;
    };

    class Material
    {
    public:
        void BuildPipeline();
        rpi::RPIPipeline GetPipeline();
    private:
        MaterialShader m_vs{};
        MaterialShader m_ps{};
        rpi::RPIPipeline m_pipeline{};
        rpi::RPIResourceBinding m_resource_binding{};
        MaterialParameterPool m_parameters_pool;
    };

    class MaterialInstance
    {
    public:
        MaterialInstance(Material* material);
    private:
        std::weak_ptr<Material> m_material;
        MaterialParameterPool m_parameters_pool;
        rpi::RPIResourceBinding m_resource_binding{};
    };

    class MaterialManager
    {

    };
}