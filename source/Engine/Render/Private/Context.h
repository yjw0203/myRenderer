#pragma once
#include "Engine/Render/Private/MaterialManager.h"
#include "Engine/Render/Private/PrimitiveManager.h"

class RdContext
{
public:
    MaterialManager m_material_manager{};
    PrimitiveManager m_primitive_manager{};
    class RenderCamera* m_render_camera{};
    float m_delta_time{};
};
