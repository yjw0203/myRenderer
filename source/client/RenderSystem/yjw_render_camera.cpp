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

    float RenderCameraInputDispatcher::speed = 0.005;

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
    }

    void RenderCameraInputDispatcher::W()
    {
        RenderCamera& camera = RenderSystem::get().activeCamera;
        camera.position += glm::normalize(camera.direction) * speed;
    }

    void RenderCameraInputDispatcher::A()
    {
        RenderCamera& camera = RenderSystem::get().activeCamera;
        glm::vec3 right = glm::normalize(glm::cross(camera.direction, camera.up));
        camera.position += (-right) * speed;
    }

    void RenderCameraInputDispatcher::S()
    {
        RenderCamera& camera = RenderSystem::get().activeCamera;
        camera.position += glm::normalize(camera.direction) * speed * -1.0f;
    }

    void RenderCameraInputDispatcher::D()
    {
        RenderCamera& camera = RenderSystem::get().activeCamera;
        glm::vec3 right = glm::normalize(glm::cross(camera.direction, camera.up));
        camera.position += right * speed;
    }

}