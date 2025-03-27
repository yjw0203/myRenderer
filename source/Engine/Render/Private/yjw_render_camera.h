#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace yjw
{
    class RenderCamera
    {
    public:
        RenderCamera() : m_position(0.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f), m_rotation(0.0f, 0.0f, 0.0f, 1.0f), fov(60.0f), zNear(0.01f), zFar(2000.0f), aspectRatio(1.0f) {};
        
        void SetPosition(glm::vec3 position);
        void SetRotation(glm::quat rotation);
        glm::vec3 position();
        glm::quat rotation();
        glm::vec3 forward() const { return (glm::vec3(0, 0, -1)) * glm::inverse(m_rotation); }
        glm::vec3 up() const { return (glm::vec3(0, 1, 0)) * glm::inverse(m_rotation); }
        glm::vec3 right() const { return (glm::vec3(1, 0, 0)) * glm::inverse(m_rotation); }
        glm::vec3 AbsoluteUp() { return m_up; }
        glm::mat4x4 getViewMatrixWithoutTranslation();
        glm::mat4x4 getViewMatrix();
        glm::mat4x4 getProjectionMatrix();
        glm::mat4x4 getViewProjectionMatrix();

    private:
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_up;
        float fov = 60.0f;//radians
        float zNear = 0.01f;
        float zFar = 2000.0f;
        float aspectRatio = 1.0f;
    };

    class RenderCameraInputDispatcher
    {
    public:
        RenderCameraInputDispatcher(class RenderSystem* render_system) :m_render_system(render_system){};
        void Register();

    private:
        float speed = 500.0f;
        float lastMouseX = 0.0f;
        float lastMouseY = 0.0f;
        bool isRightMouseButton = false;

        void W();
        void A();
        void S();
        void D();
        void Q();
        void E();
        void RightMouseDown();
        void MouseMove(float x, float y);
        void RightMouseRelease();
        void MouseScroll(float x, float y);

        class RenderSystem* m_render_system = nullptr;
    };
}