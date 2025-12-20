#include "Engine/Oracle/Public/Widgets/OcPanel.h"

void OcPanel::OnPreComputeDesiredSize(float ScaleMultiplier)
{
    for (OcSlotHandle& slot : m_slots)
    {
        if (slot.get())
        {
            if (slot->IsBounding())
            {
                slot->GetSlotWidget()->ComputeDesiredSize(ScaleMultiplier);
            }
        }
    }
}

void OcPanel::OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets)
{
    for (OcSlotHandle slot_handle : m_slots)
    {
        if (slot_handle && slot_handle->IsBounding())
        {
            Vector2 offset = slot_handle->ActualPosition(Layout);
            Vector2 size = slot_handle->ActualSize(Layout);

            LayoutedWidgets.AddWidget(Layout.MakeChild(offset, size), slot_handle->GetSlotWidget());
        }
    }
}