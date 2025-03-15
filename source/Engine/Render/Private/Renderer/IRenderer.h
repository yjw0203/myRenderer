#pragma once

namespace yjw
{
    class IRenderer
    {
    public:
        IRenderer() {}
        virtual void Submit(RenderEntity* entity) = 0;
    };
}