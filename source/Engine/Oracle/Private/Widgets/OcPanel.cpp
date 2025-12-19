#include "Engine/Oracle/Public/Widgets/OcPanel.h"

void OcPanel::OnPreComputeDesiredSize(float ScaleMultiplier)
{
    for (OracleWidgetSlotHandle& slot : m_slots)
    {
        if (slot.get())
        {
            if (slot.get()->IsBounding())
            {
                slot.get()->GetBounding()->ComputeDesiredSize(ScaleMultiplier);
            }
        }
    }
}

OracleWidgetSlotHandle OcPanel::AddSubWidget(OcWidgetHandle widget)
{
    OracleWidgetSlotHandle slot = OcMakeShared<OracleWidgetSlot>(this);
    slot.get()->SetSlotWidget(widget);
    SetOwnedSlot(slot);
    m_slots.push_back(slot);
    return slot;
}
