#pragma once
#include "glm/glm.hpp"

namespace yjw
{
    struct Bezier
    {
        float EvalX(float t) const;
        float EvalY(float t) const;
        glm::vec2 Eval(float t) const;

        float FindBezierX(float time) const;

        glm::vec2	m_cp1;
        glm::vec2	m_cp2;
    };
}