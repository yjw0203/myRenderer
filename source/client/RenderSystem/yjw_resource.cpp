#include "yjw_resource.h"
#include "yjw_render_system.h"

namespace yjw
{
    GlobalResourceStore g_resource_store;

    void GlobalResourceStore::initializeResource()
    {
        cameraUniform = std::make_shared<RHIUniformBuffer>(144);
        lightUniform = std::make_shared<RHIUniformBuffer>(32);
        optionUniform = std::make_shared<RHIUniformBuffer>(16);

        glm::vec2 screenSize = glm::vec2(720, 720);
        optionUniform->update(&screenSize, sizeof(screenSize), 0);
    }

    void GlobalResourceStore::updateCameraData()
    {
        RenderCamera& camera = RenderSystem::get().activeCamera;
        struct Data
        {
            glm::mat4x4 view;
            glm::mat4x4 project;
            glm::vec4 positon;
        }data;

        data.view = camera.getViewMatrix();
        data.project = camera.getProjectionMatrix();
        data.positon = glm::vec4(camera.position, 1);
        cameraUniform->update(&data, sizeof(data), 0);
    }
    
    void GlobalResourceStore::updateLightData()
    {
        struct Data
        {
            alignas(16) glm::vec3 pos;
            alignas(16) glm::vec3 color;
        };
        Data light;
        light.pos = glm::vec3(-3, 15, -8);
        light.color = glm::vec3(1, 1, 1);
        lightUniform->update(&light, sizeof(light), 0);
    }

}