#pragma once
#include "glm/glm.hpp"

namespace yjw
{
    class Light
    {
    public:
        glm::vec3 m_direction;
        glm::vec3 m_color;
    };
}