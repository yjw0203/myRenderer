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
        void SetOnMousePress(std::function<void(float, float)> func);
        void SetOnMouseRelease(std::function<void(float, float)> func);
        void SetOnMouseMove(std::function<void(float, float)> func);
        void SetOnQ(std::function<void()> func);
        void SetOnW(std::function<void()> func);
        void SetOnE(std::function<void()> func);
    private:
        float m_lastPressX = 0.0;
        float m_lastPressY = 0.0;

        float m_lastMouseX = 0.0f;
        float m_lastMouseY = 0.0f;
        bool m_isLeftMouseButton = false;

        void LeftMouseDown();
        void MouseMove(float x, float y);
        void LeftMouseRelease();
        void Q();
        void W();
        void E();

        std::function<void(float,float)> m_clicked;
        std::function<void(float,float)> m_mouse_press;
        std::function<void(float,float)> m_mouse_release;
        std::function<void(float,float)> m_mouse_move;
        std::function<void()> m_q;
        std::function<void()> m_w;
        std::function<void()> m_e;
    };
}