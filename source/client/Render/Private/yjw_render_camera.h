#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace yjw
{
    class RenderCamera
    {
    public:
        RenderCamera() : position(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 1.0f), up(0.0f, 1.0f, 0.0f), fov(60.0f), zNear(0.01f), zFar(2000.0f), aspectRatio(1.0f) {};
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;
        float fov = 60.0f;//radians
        float zNear = 0.01f;
        float zFar = 2000.0f;
        float aspectRatio = 1.0f;

        glm::mat4x4 getViewMatrix();
        glm::mat4x4 getProjectionMatrix();
        glm::mat4x4 getViewProjectionMatrix();
    };

    class RenderCameraInputDispatcher
    {
    public:
        static void Register();

    private:
        static float speed;
        static float lastMouseX;
        static float lastMouseY;
        static bool isRightMouseButton;

        static void W();
        static void A();
        static void S();
        static void D();
        static void Q();
        static void E();
        static void RightMouseDown();
        static void MouseMove(float x, float y);
        static void RightMouseRelease();
        static void MouseScroll(float x, float y);

    };
}