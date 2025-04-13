#pragma once

namespace yjw
{
    struct RenderHitRequest
    {
        int m_hit_flag{};
        int m_posx{};
        int m_posy{};
        bool m_completed = false;
        int m_result[4] = {};
    };
}