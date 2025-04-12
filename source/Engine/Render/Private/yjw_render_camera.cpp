#include "yjw_render_camera.h"
#include "Engine/Render/Public/yjw_render_system.h"
#include "Engine/Engine/Public/Window.h"

namespace yjw
{
    void RenderCamera::SetPosition(glm::vec3 position)
    {
        m_position = position;
    }

    void RenderCamera::SetRotation(glm::quat rotation)
    {
        m_rotation = rotation;
    }

    glm::vec3 RenderCamera::position()
    {
        return m_position;
    }

    glm::quat RenderCamera::rotation()
    {
        return m_rotation;
    }

    glm::mat4x4 RenderCamera::getViewMatrixWithoutTranslation()
    {
        return glm::lookAt(glm::vec3(0,0,0), forward(), up());
    }

    glm::mat4x4 RenderCamera::getViewMatrix()
    {
        return glm::lookAt(m_position, m_position + forward(), up());
    }

    glm::mat4x4 RenderCamera::getProjectionMatrix()
    {
        return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }

    glm::mat4x4 RenderCamera::getViewProjectionMatrix()
    {
        return getViewMatrix() * getProjectionMatrix();
    }

    void RenderCameraInputDispatcher::Register()
    {
        InputDispatcher::registerKeyEvent(GLFW_KEY_W, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::W, this));
        InputDispatcher::registerKeyEvent(GLFW_KEY_W, GLFW_REPEAT, 0, std::bind(&RenderCameraInputDispatcher::W, this));

        InputDispatcher::registerKeyEvent(GLFW_KEY_A, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::A, this));
        InputDispatcher::registerKeyEvent(GLFW_KEY_A, GLFW_REPEAT, 0, std::bind(&RenderCameraInputDispatcher::A, this));

        InputDispatcher::registerKeyEvent(GLFW_KEY_S, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::S, this));
        InputDispatcher::registerKeyEvent(GLFW_KEY_S, GLFW_REPEAT, 0, std::bind(&RenderCameraInputDispatcher::S, this));

        InputDispatcher::registerKeyEvent(GLFW_KEY_D, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::D, this));
        InputDispatcher::registerKeyEvent(GLFW_KEY_D, GLFW_REPEAT, 0, std::bind(&RenderCameraInputDispatcher::D, this));

        InputDispatcher::registerKeyEvent(GLFW_KEY_Q, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::Q, this));
        InputDispatcher::registerKeyEvent(GLFW_KEY_Q, GLFW_REPEAT, 0, std::bind(&RenderCameraInputDispatcher::Q, this));

        InputDispatcher::registerKeyEvent(GLFW_KEY_E, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::E, this));
        InputDispatcher::registerKeyEvent(GLFW_KEY_E, GLFW_REPEAT, 0, std::bind(&RenderCameraInputDispatcher::E, this));

        InputDispatcher::registerMouseButtonEvent(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, 0, std::bind(&RenderCameraInputDispatcher::RightMouseDown, this));
        InputDispatcher::registerMousePosEvent(std::bind(&RenderCameraInputDispatcher::MouseMove, this, std::placeholders::_1, std::placeholders::_2));
        InputDispatcher::registerMouseButtonEvent(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE, 0, std::bind(&RenderCameraInputDispatcher::RightMouseRelease, this));
        InputDispatcher::registerMouseScrollEvent(std::bind(&RenderCameraInputDispatcher::MouseScroll, this, std::placeholders::_1, std::placeholders::_2));
    }

    void RenderCameraInputDispatcher::W()
    {
        float deltaTime = m_render_system->m_delta_time;
        RenderCamera& camera = *m_render_system->activeCamera;
        glm::vec3 relative_pos = glm::vec3(0, 0, -speed * deltaTime) * glm::inverse(camera.rotation());
        camera.SetPosition(camera.position() + relative_pos);
    }

    void RenderCameraInputDispatcher::A()
    {
        float deltaTime = m_render_system->m_delta_time;
        RenderCamera& camera = *m_render_system->activeCamera;
        glm::vec3 relative_pos = glm::vec3(-speed * deltaTime, 0, 0) * glm::inverse(camera.rotation());
        camera.SetPosition(camera.position() + relative_pos);
    }

    void RenderCameraInputDispatcher::S()
    {
        float deltaTime = m_render_system->m_delta_time;
        RenderCamera& camera = *m_render_system->activeCamera;
        glm::vec3 relative_pos = glm::vec3(0, 0, speed * deltaTime) * glm::inverse(camera.rotation());
        camera.SetPosition(camera.position() + relative_pos);
    }

    void RenderCameraInputDispatcher::D()
    {
        float deltaTime = m_render_system->m_delta_time;
        RenderCamera& camera = *m_render_system->activeCamera;
        glm::vec3 relative_pos = glm::vec3(speed * deltaTime, 0, 0) * glm::inverse(camera.rotation());
        camera.SetPosition(camera.position() + relative_pos);
    }

    void RenderCameraInputDispatcher::Q()
    {
        float deltaTime = m_render_system->m_delta_time;
        RenderCamera& camera = *m_render_system->activeCamera;
        glm::vec3 relative_pos = glm::vec3( 0, speed * deltaTime, 0) * glm::inverse(camera.rotation());
        camera.SetPosition(camera.position() + relative_pos);
    }

    void RenderCameraInputDispatcher::E()
    {
        float deltaTime = m_render_system->m_delta_time;
        RenderCamera& camera = *m_render_system->activeCamera;
        glm::vec3 relative_pos = glm::vec3(0, -speed * deltaTime, 0) * glm::inverse(camera.rotation());
        camera.SetPosition(camera.position() + relative_pos);
    }

    void RenderCameraInputDispatcher::RightMouseDown()
    {
        isRightMouseButton = true;
    }

    void RenderCameraInputDispatcher::MouseMove(float x, float y)
    {
        if (isRightMouseButton)
        {
            float angularVelocity = glm::radians(180.0f);
            float deltaX = (x - lastMouseX) / 1200 * angularVelocity;
            float deltaY = (y - lastMouseY) / 1200 * angularVelocity;
            RenderCamera& camera = *m_render_system->activeCamera;
            glm::quat camera_rot = camera.rotation();

            glm::vec3 XAxis = glm::vec3(1, 0, 0);
            glm::vec3 pitchAxis = XAxis;
            camera_rot = glm::rotate(camera_rot, deltaY, pitchAxis);

            glm::vec3 YAxis = glm::vec3(0, 1, 0) * camera_rot;
            glm::vec3 yawAxis = YAxis;
            camera_rot = glm::rotate(camera_rot, deltaX, yawAxis);

            camera.SetRotation(camera_rot);
        }
        lastMouseX = x;
        lastMouseY = y;
    }

    void RenderCameraInputDispatcher::RightMouseRelease()
    {
        isRightMouseButton = false;
    }

    void RenderCameraInputDispatcher::MouseScroll(float x, float y)
    {
        speed += y * 10.f;
        if (speed > 5000.0f)speed = 5000.0f;
        if (speed < 10.f)speed = 10.f;
    }

}