#include "yjw_resource.h"
#include "yjw_render_system.h"

namespace yjw
{
    GlobalResourceStore g_resource_store;

    void GlobalResourceStore::initializeResource()
    {
        cameraUniform = RPICreateUploadBuffer(144);
        lightUniform = RPICreateUploadBuffer(32);
        optionUniform = RPICreateUploadBuffer(16);

        cameraUniformDescriptor = RPICreateBufferView(cameraUniform, 0, 144);
        lightUniformDescriptor = RPICreateBufferView(lightUniform, 0, 32);
        optionUniformDescriptor = RPICreateBufferView(optionUniform, 0, 16);

        glm::vec2 screenSize = glm::vec2(1200, 1200);
        RPIUpdateBuffer(optionUniform, &screenSize, 0, sizeof(screenSize));
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
        RPIUpdateBuffer(cameraUniform, &data, 0, sizeof(data));
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
        RPIUpdateBuffer(lightUniform, &light, 0, sizeof(light));
    }

}