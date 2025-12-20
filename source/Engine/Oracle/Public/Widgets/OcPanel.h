#pragma once
#include "Engine/Oracle/Public/Widgets/OcWidget.h"
#include "Engine/Oracle/Public/Layout/OracleWidgetSlot.h"

class OcPanel : public OcWidget
{
public:
    virtual void OnPreComputeDesiredSize(float ScaleMultiplier) override;
    virtual void OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets) override;

protected:
    template<typename SlotType>
    OcPtr<SlotType> AddSubWidget(OcWidgetHandle widget)
    {
        OcPtr<SlotType> slot = OcMakeShared<SlotType>();
        slot.get()->SetSlotWidget(widget);
        m_slots.push_back(slot);
        return slot;
    }

public:
    OcArray<OcSlotHandle> m_slots;
};
