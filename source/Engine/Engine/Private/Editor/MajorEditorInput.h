#pragma once
#include <functional>

namespace yjw
{
    class MajorInputDispatcher
    {
    public:
        MajorInputDispatcher() {};
        void Register();
        void SetOnClicked(std::function<void(float, float)> func);
    private:
        float m_lastMouseX = 0.0f;
        float m_lastMouseY = 0.0f;
        bool m_isLeftMouseButton = false;

        void LeftMouseDown();
        void MouseMove(float x, float y);
        void LeftMouseRelease();

        std::function<void(float,float)> m_onclicked;
    };
}