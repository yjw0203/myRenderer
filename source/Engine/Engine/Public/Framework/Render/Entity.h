#pragma once

namespace yjw
{
    using MaterialHandle = int;
    using MeshHandle = int;
    using EntityHandle = int;
    using ViewHandle = int;

    enum RenderMaskBits
    {
        highlight = 1 << 0
    };

    using RenderMask = int;
}