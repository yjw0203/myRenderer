#pragma once
#include "Engine/Asset/Public/Asset.h"

namespace yjw
{
    CAsset(MaterialAST)
    {
    public:
        std::string m_shader;
        std::string m_entry;
    };

    CAsset(MaterialInstanceAST)
    {
    public:
        AssetReferece<MaterialAST> m_material_template; 

        std::map<std::string, float> m_float_params;
        //std::map<std::string, glm::vec2> m_vec2_params;
        //std::map<std::string, glm::vec3> m_vec3_params;
        //std::map<std::string, glm::vec4> m_vec4_params;
        //std::map<std::string, glm::mat4x4> m_mat4x4_params;
        std::map<std::string, std::string> m_texture_params;
    };

}