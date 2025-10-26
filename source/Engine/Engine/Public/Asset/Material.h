#pragma once
#include "Engine/Asset/Public/Asset.h"

namespace yjw
{
    class Meta() MaterialAST : public MObject
    {
        GENERATED_BODY();
    public:
        Meta()
        std::string m_shader;
        Meta()
        std::string m_entry;
    };

    class Meta() MaterialInstanceAST : public MObject
    {
        GENERATED_BODY();
    public:
        MaterialInstanceAST(){}
        Meta()
        AssetReferece<MaterialAST> m_material_template; 

        Meta()
        std::map<std::string, float> m_float_params;
        //std::map<std::string, glm::vec2> m_vec2_params;
        //std::map<std::string, glm::vec3> m_vec3_params;
        //std::map<std::string, glm::vec4> m_vec4_params;
        //std::map<std::string, glm::mat4x4> m_mat4x4_params;
        Meta()
        std::map<std::string, std::string> m_texture_params;
    };

}