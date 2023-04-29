#include "yjw_render_camera.h"

namespace yjw
{
    glm::mat4x4 RenderCamera::getViewMatrix()
    {
        return glm::lookAt(position, position + direction, up);
    }

    glm::mat4x4 RenderCamera::getProjectionMatrix()
    {
        return glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    glm::mat4x4 RenderCamera::getViewProjectionMatrix()
    {
        return getViewMatrix() * getProjectionMatrix();
    }
}