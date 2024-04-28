#pragma once
#include "3rd/imgui.h"

namespace rhi
{
    class ImGuiUI
    {
    public:
        virtual void DecodeUI() = 0;
    };
}