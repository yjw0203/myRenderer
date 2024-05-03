#pragma once
#include "native/imgui.h"

namespace rhi
{
    class ImGuiUI
    {
    public:
        virtual void DecodeUI() = 0;
    };
}