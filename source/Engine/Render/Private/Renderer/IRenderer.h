#pragma once

namespace yjw
{
    class IRenderer
    {
    public:
        IRenderer() {}
        virtual void Submit(class DrawItem* entity) = 0;
    };
}