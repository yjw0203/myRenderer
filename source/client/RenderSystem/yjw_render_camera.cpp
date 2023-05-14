#include "yjw_render_camera.h"
#include "yjw_render_system.h"

namespace yjw
{
    glm::mat4x4 RenderCamera::getViewMatrix()
    {
        return glm::lookAt(position, position + direction, up);
    }

    glm::mat4x4 RenderCamera::getProjectionMatrix()
    {
        return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    }

    glm::mat4x4 RenderCamera::getViewProjectionMatrix()
    {
        return getProjectionMatrix() * getViewMatrix();
    }

    float RenderCameraInputDispatcher::speed = 100;
    float RenderCameraInputDispatcher::lastMouseX = 0.00;
    float RenderCameraInputDispatcher::lastMouseY = 0.00;
    bool RenderCameraInputDispatcher::isRightMouseButton = false;

    void RenderCameraInputDispatcher::Register()
    {
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_W, GLFW_PRESS, 0, W);
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_W, GLFW_REPEAT, 0, W);

        InputDispatcher::get().registerKeyEvent(GLFW_KEY_A, GLFW_PRESS, 0, A);
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_A, GLFW_REPEAT, 0, A);

        InputDispatcher::get().registerKeyEvent(GLFW_KEY_S, GLFW_PRESS, 0, S);
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_S, GLFW_REPEAT, 0, S);

        InputDispatcher::get().registerKeyEvent(GLFW_KEY_D, GLFW_PRESS, 0, D);
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_D, GLFW_REPEAT, 0, D);

        InputDispatcher::get().registerKeyEvent(GLFW_KEY_Q, GLFW_PRESS, 0, Q);
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_Q, GLFW_REPEAT, 0, Q);

        InputDispatcher::get().registerKeyEvent(GLFW_KEY_E, GLFW_PRESS, 0, E);
        InputDispatcher::get().registerKeyEvent(GLFW_KEY_E, GLFW_REPEAT, 0, E);

        InputDispatcher::get().registerMouseButtonEvent(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, 0, RightMouseDown);
        InputDispatcher::get().registerMousePosEvent(MouseMove);
        InputDispatcher::get().registerMouseButtonEvent(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE, 0, RightMouseRelease);
        InputDispatcher::get().registerMouseScrollEvent(MouseScroll);
    }

    void RenderCameraInputDispatcher::W()
    {
        float deltaTime = RenderSystem::get().deltaTime;
        RenderCamera& camera = RenderSystem::get().activeCamera;
        camera.position += glm::normalize(camera.direction) * speed * deltaTime;
    }

    void RenderCameraInputDispatcher::A()
    {
        float deltaTime = RenderSystem::get().deltaTime;
        RenderCamera& camera = RenderSystem::get().activeCamera;
        glm::vec3 right = glm::normalize(glm::cross(camera.direction, camera.up));
        camera.position += (-right) * speed * deltaTime;
    }

    void RenderCameraInputDispatcher::S()
    {
        float deltaTime = RenderSystem::get().deltaTime;
        RenderCamera& camera = RenderSystem::get().activeCamera;
        camera.position += glm::normalize(camera.direction) * speed * -1.0f * deltaTime;
    }

    void RenderCameraInputDispatcher::D()
    {
        float deltaTime = RenderSystem::get().deltaTime;
        RenderCamera& camera = RenderSystem::get().activeCamera;
        glm::vec3 right = glm::normalize(glm::cross(camera.direction, camera.up));
        camera.position += right * speed * deltaTime;
    }

    void RenderCameraInputDispatcher::Q()
    {
        float deltaTime = RenderSystem::get().deltaTime;
        RenderCamera& camera = RenderSystem::get().activeCamera;
        camera.position += glm::normalize(camera.up) * speed * -1.0f * deltaTime;
    }

    void RenderCameraInputDispatcher::E()
    {
        float deltaTime = RenderSystem::get().deltaTime;
        RenderCamera& camera = RenderSystem::get().activeCamera;
        camera.position += glm::normalize(camera.up) * speed * deltaTime;
    }

    void RenderCameraInputDispatcher::RightMouseDown()
    {
        isRightMouseButton = true;
    }

    void RenderCameraInputDispatcher::MouseMove(float x, float y)
    {
        if (isRightMouseButton)
        {
            float deltaX = (lastMouseX - x) * 0.1;
            float deltaY = (y - lastMouseY) * 0.1;

            RenderCamera& camera = RenderSystem::get().activeCamera;

            glm::vec3 direction = glm::normalize(camera.direction);
            glm::vec3 up = glm::normalize(camera.up);
            glm::vec3 right = glm::cross(camera.direction, camera.up);
            
            glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(deltaY), right);
            camera.direction = glm::vec3(rotMat * glm::vec4(camera.direction, 0.0f));
            camera.up = glm::vec3(rotMat * glm::vec4(camera.up, 0.0f));

            
            rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(deltaX), up);
            camera.direction = glm::vec3(rotMat * glm::vec4(camera.direction, 0.0f));
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