#pragma once
#include "native/imgui.h"
#include "native/imgui_internal.h"

namespace rhi
{
    class ImGuiUI
    {
    public:
        virtual void DecodeUI() = 0;
    };
}