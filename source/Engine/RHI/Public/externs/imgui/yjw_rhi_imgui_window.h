#pragma once
#include "Engine/RHI/Public/externs/imgui/native/imgui.h"
#include "Engine/RHI/Public/externs/imgui/native/imgui_internal.h"
#include "Engine/RHI/Public/externs/imgui/native/ImGuizmo.h"
#include "Engine/RHI/Public/externs/imgui/native/ImSequencer.h"
#include "Engine/RHI/Public/externs/imgui/native/ImGradient.h"
#include "Engine/RHI/Public/externs/imgui/native/ImCurveEdit.h"

namespace rhi
{
    class ImGuiUI
    {
    public:
        virtual void DecodeUI() = 0;
    };
}