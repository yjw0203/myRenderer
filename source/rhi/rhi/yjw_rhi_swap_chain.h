#pragma once

namespace rhi
{
    class RHISwapChain
    {
    public:
        virtual void Prensent(bool bSync) = 0;
    };
}