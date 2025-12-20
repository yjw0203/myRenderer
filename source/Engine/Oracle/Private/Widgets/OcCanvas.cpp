#include "Engine/Oracle/Public/Widgets/OcCanvas.h"

void OcCanvas::OnComputeDesiredSize(float ScaleMultiplier)
{
    Vector2 desired_size = Vector2(0, 0);

    SetDesiredSize(desired_size);
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

OcAnchorSlot& OcCanvas::AddSubWidget(OcWidgetHandle widget)
{
    return *OcPanel::AddSubWidget<OcAnchorSlot>(widget);
}

OcAnchorSlot::Argument OcCanvas::Slot()
{
    OcPtr<OcAnchorSlot> slot = OcMakeShared<OcAnchorSlot>();
    return OcAnchorSlot::Argument{ slot};
}
