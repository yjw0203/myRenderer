#pragma once
#include "Vector.h"
namespace yjw
{
    class Transform
    {
    public:
        Transform() :m_location(0), m_scale(1), m_rotate(1, 0, 0, 0) {}
        Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) :
            m_location{ position }, m_scale{ scale }, m_rotate{ rotation }
        {}

        Matrix4x4 getMatrix() const
        {
            auto s = glm::scale(glm::mat4(1), m_scale);
            auto r = glm::mat4_cast(m_rotate);
            auto t = glm::translate(glm::mat4(1), m_location);
            Matrix4x4 mat = t * r * s;
            return mat;
        }
        Vector3 m_location;
        Vector3 m_scale;
        Quaternion m_rotate;
    };
}