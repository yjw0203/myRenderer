#pragma once
#include "Engine/Oracle/Public/Widgets/OcPanel.h"

class OcCanvas : public OcPanel
{
public:
    virtual void OnComputeDesiredSize(float ScaleMultiplier) override;
    virtual void OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout) override;

    OcAnchorSlot& AddSubWidget(OcWidgetHandle widget);

    static OcAnchorSlot::Argument Slot();

    struct Argument : OcWidgetArgument
    {
        Argument& Style(OcStyleHandle& style) {  m_widget->m_style = style; return *this; }
        
        Argument& operator + (OcAnchorSlot::Argument& slot)
        {
            ((OcCanvas*)m_widget.get())->m_slots.push_back(slot.slot);
            return *this;
        }
    };
};
