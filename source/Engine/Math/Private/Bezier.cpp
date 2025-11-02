#include "Engine/Math/Public/Bezier.h"

float Bezier::EvalX(float t) const
{
    const float t2 = t * t;
    const float t3 = t2 * t;
    const float it = 1.0f - t;
    const float it2 = it * it;
    const float it3 = it2 * it;
    const float x[4] = {
        0,
        m_cp1.x,
        m_cp2.x,
        1,
    };

    return t3 * x[3] + 3 * t2 * it * x[2] + 3 * t * it2 * x[1] + it3 * x[0];
}

float Bezier::EvalY(float t) const
{
    const float t2 = t * t;
    const float t3 = t2 * t;
    const float it = 1.0f - t;
    const float it2 = it * it;
    const float it3 = it2 * it;
    const float y[4] = {
        0,
        m_cp1.y,
        m_cp2.y,
        1,
    };

    return t3 * y[3] + 3 * t2 * it * y[2] + 3 * t * it2 * y[1] + it3 * y[0];
}

glm::vec2 Bezier::Eval(float t) const
{
    return glm::vec2(EvalX(t), EvalY(t));
}

float Bezier::FindBezierX(float time) const
{
    const float e = 0.00001f;
    float start = 0.0f;
    float stop = 1.0f;
    float t = 0.5f;
    float x = EvalX(t);
    while (std::abs(time - x) > e)
    {
        if (time < x)
        {
            stop = t;
        }
        else
        {
            start = t;
        }
        t = (stop + start) * 0.5f;
        x = EvalX(t);
    }

    return t;
}
