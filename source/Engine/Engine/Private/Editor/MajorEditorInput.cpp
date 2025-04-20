#include "Engine/Engine/Private/Editor/MajorEditorInput.h"
#include "Engine/Engine/Public/Window.h"

namespace yjw
{
    void MajorInputDispatcher::Register()
    {
        InputDispatcher::registerMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0, std::bind(&MajorInputDispatcher::LeftMouseDown, this));
        InputDispatcher::registerMousePosEvent(std::bind(&MajorInputDispatcher::MouseMove, this, std::placeholders::_1, std::placeholders::_2));
        InputDispatcher::registerMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0, std::bind(&MajorInputDispatcher::LeftMouseRelease, this));
        InputDispatcher::registerMouseButtonEvent(GLFW_KEY_Q, GLFW_PRESS, 0, std::bind(&MajorInputDispatcher::Q, this));
        InputDispatcher::registerMouseButtonEvent(GLFW_KEY_W, GLFW_PRESS, 0, std::bind(&MajorInputDispatcher::W, this));
        InputDispatcher::registerMouseButtonEvent(GLFW_KEY_E, GLFW_PRESS, 0, std::bind(&MajorInputDispatcher::E, this));
    }

    void MajorInputDispatcher::SetOnClicked(std::function<void(float, float)> func)
    {
        m_clicked = func;
    }

    void MajorInputDispatcher::SetOnMousePress(std::function<void(float, float)> func)
    {
        m_mouse_press = func;
    }

    void MajorInputDispatcher::SetOnMouseRelease(std::function<void(float, float)> func)
    {
        m_mouse_release = func;
    }

    void MajorInputDispatcher::SetOnMouseMove(std::function<void(float, float)> func)
    {
        m_mouse_move = func;
    }

    void MajorInputDispatcher::SetOnQ(std::function<void()> func)
    {
        m_q = func;
    }

    void MajorInputDispatcher::SetOnW(std::function<void()> func)
    {
        m_w = func;
    }

    void MajorInputDispatcher::SetOnE(std::function<void()> func)
    {
        m_e = func;
    }

    void MajorInputDispatcher::LeftMouseDown()
    {
        m_isLeftMouseButton = true;
        m_lastPressX = m_lastMouseX;
        m_lastPressY = m_lastMouseY;
        m_mouse_press(m_lastMouseX, m_lastMouseY);
    }

    void MajorInputDispatcher::MouseMove(float x, float y)
    {
        m_lastMouseX = x;
        m_lastMouseY = y;
        m_mouse_move(x, y);
    }

    void MajorInputDispatcher::LeftMouseRelease()
    {
        m_isLeftMouseButton = false;
        m_mouse_release(m_lastMouseX, m_lastMouseY);
        if (m_lastMouseX == m_lastPressX && m_lastMouseY == m_lastPressY)
        {
            m_clicked(m_lastPressX, m_lastPressY);
        }
    }

    void MajorInputDispatcher::Q()
    {
        m_q();
    }

    void MajorInputDispatcher::W()
    {
        m_w();
    }

    void MajorInputDispatcher::E()
    {
        m_e();
    }
}