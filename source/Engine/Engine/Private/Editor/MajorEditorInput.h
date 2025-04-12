#pragma once

namespace yjw
{
    class MajorInputDispatcher
    {
    public:
    public:
        MajorInputDispatcher() {};
        void Register();

    private:
        float m_lastMouseX = 0.0f;
        float m_lastMouseY = 0.0f;
        bool m_isLeftMouseButton = false;

        void LeftMouseDown();
        void MouseMove(float x, float y);
        void LeftMouseRelease();
    };
}