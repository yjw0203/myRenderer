#pragma once

namespace yjw
{
    class Pass
    {
    public:
        virtual void buildPSO() = 0;
        virtual void setupData() = 0;
        virtual void recordCommand() = 0;
        virtual void submit() = 0;
    };
}