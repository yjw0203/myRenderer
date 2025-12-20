#include "Engine/Oracle/Public/Layout/OracleWidgetSlot.h"

void OcSlot::SetSlotWidget(OcWidgetHandle widget)
{
    m_widget = widget;
}

OcWidgetHandle OcSlot::GetSlotWidget()
{
    return m_widget;
}

void OcSlot::RemoveSlot(OcWidgetHandle widget)
{
    m_widget = nullptr;
}

bool OcSlot::IsBounding()
{
    return m_widget.get() != nullptr;
}

Vector2 OcAnchorSlot::ActualPosition(const OracleLayout& Layout)
{
    return Layout.m_size * m_anchor.Minimum() + m_left_top;
}

Vector2 OcAnchorSlot::ActualSize(const OracleLayout& Layout)
{
    float SizeX = m_right_bottom.x;
    float SizeY = m_right_bottom.y;
    if (m_anchor.Maximum().x != m_anchor.Minimum().x)
    {
        SizeX = Layout.m_size.x * (m_anchor.Maximum().x - m_anchor.Minimum().x) - m_right_bottom.x - m_left_top.x;
    }
    if (m_anchor.Maximum().y != m_anchor.Minimum().y)
    {
        SizeY = Layout.m_size.y * (m_anchor.Maximum().y - m_anchor.Minimum().y) - m_right_bottom.y - m_left_top.y;
    }

    return Vector2(SizeX, SizeY);
}

OcAnchorSlot& OcAnchorSlot::LeftTopRightBottom(Vector4 size)
{
    m_left_top = Vector2(size.x, size.y);
    m_right_bottom = Vector2(size.z, size.w);
    return *this;
}

OcAnchorSlot& OcAnchorSlot::LeftTopRightBottom(float left, float top, float right, float bottom)
{
    m_left_top = Vector2(left, top);
    m_right_bottom = Vector2(right, bottom);
    return *this;
}

OcAnchorSlot& OcAnchorSlot::Anchor(FAnchor anchor)
{
    m_anchor = anchor;
    return *this;
}

const FAnchor& OcAnchorSlot::Anchor() const
{
    return m_anchor;
}