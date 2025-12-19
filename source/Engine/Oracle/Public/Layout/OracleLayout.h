#pragma once
#include "Engine/Oracle/Public/Utils/Utils.h"

class OracleLayout
{
public:
    Vector2 m_absolute_position{};
    Vector2 m_size{};
    float m_scale{ 1.0 };

    OracleLayout()
        : m_absolute_position(0, 0)
        , m_size(0, 0)
        , m_scale(1.0f)
    { }

    OracleLayout(Vector2 position, Vector2 size, float scale)
        : m_absolute_position(position)
        , m_size(size)
        , m_scale(scale)
    { }

    OracleLayout MakeChild(Vector2 offset, Vector2 size) const
    {
        return OracleLayout(m_absolute_position + offset, size, 1.0f);
    }
};

class OcWidget;
using OcWidgetHandle = OcPtr<OcWidget>;
class OracleLayoutedWidgets
{
public:
    void AddWidget(const OracleLayout& layout, OcWidgetHandle widget);
    OcArray<OcPair<OracleLayout, OcWidgetHandle>>& GetArray();
private:
    OcArray<OcPair<OracleLayout, OcWidgetHandle>> m_layouted_widgets;
};