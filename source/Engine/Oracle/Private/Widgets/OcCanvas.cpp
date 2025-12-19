#include "Engine/Oracle/Public/Widgets/OcCanvas.h"

void OcCanvas::OnComputeDesiredSize(float ScaleMultiplier)
{
    OracleWidgetSlotHandle slot = GetOwnedSlot();
    Vector2 desired_size = Vector2(0, 0);
    
    if (slot.get())
    {
        desired_size = slot.get()->GetSize();
    }

    SetDesiredSize(desired_size);
}

void OcCanvas::OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets)
{
    for (OracleWidgetSlotHandle slot_handle : m_slots)
    {
        if (slot_handle)
        {
            Vector2 offset = slot_handle.get()->GetOffset();
            Vector2 size = slot_handle.get()->GetSize();

            LayoutedWidgets.AddWidget(Layout.MakeChild(offset, size), slot_handle.get()->GetSlotWidget());
        }
    }
}

void OcCanvas::OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout)
{
    //test
    OracleDrawElement::MakeBox(DrawElements, m_style.get() ? *m_style : OracleStyle(), Layout);

    OracleLayoutedWidgets LayoutedWidgets;
    LayoutChildren(Layout, LayoutedWidgets);

    for (OcPair<OracleLayout, OcWidgetHandle>& widget : LayoutedWidgets.GetArray())
    {
        if (widget.second)
        {
            widget.second->Paint(DrawElements, widget.first);
        }
    }

}
