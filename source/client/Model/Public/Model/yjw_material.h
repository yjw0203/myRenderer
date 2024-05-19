#pragma once
#include <memory>
#include <vector>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

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
    private:
        MaterialShader vs;
        MaterialShader ps;
        MaterialParameterPool m_parameters_pool;
    };

    class MaterialInstance
    {
    private:
        std::weak_ptr<Material> m_material;
        MaterialParameterPool m_parameters_pool;
    };

    class MaterialManager
    {

    };
}