#pragma once
#include "Engine/Engine/Private/Editor/MajorEditorInput.h"

namespace yjw
{
    class MajorEditor
    {
    public:
        void Startup();
    private:
        MajorInputDispatcher m_input_dispatcher{};
    };
}