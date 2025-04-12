#include "Engine/Engine/Private/Editor/MajorEditorInput.h"
#include "Engine/Engine/Public/Window.h"

namespace yjw
{
    void MajorInputDispatcher::Register()
    {
        InputDispatcher::registerMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0, std::bind(&MajorInputDispatcher::LeftMouseDown, this));
        InputDispatcher::registerMousePosEvent(std::bind(&MajorInputDispatcher::MouseMove, this, std::placeholders::_1, std::placeholders::_2));
        InputDispatcher::registerMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0, std::bind(&MajorInputDispatcher::LeftMouseRelease, this));
    }

    void MajorInputDispatcher::LeftMouseDown()
    {
        m_isLeftMouseButton = true;
    }

    void MajorInputDispatcher::MouseMove(float x, float y)
    {
        m_lastMouseX = x;
        m_lastMouseY = y;
    }

    void MajorInputDispatcher::LeftMouseRelease()
    {
        m_isLeftMouseButton = false;
        printf("%f %f\n", m_lastMouseX, m_lastMouseY);

    }
}