#include "Engine/Oracle/Public/Widgets/OcSlotWidget.h"

OcSlotWidget::OcSlotWidget()
{
    //m_slot = OcMakeShared<OcSlotHandle>(this);
}

void OcSlotWidget::OnPreComputeDesiredSize(float ScaleMultiplier)
{
    /*
    if (m_slot && m_slot->IsBounding())
    {
        m_slot->GetSlotWidget()->ComputeDesiredSize(ScaleMultiplier);
    }
    */
}

void OcSlotWidget::SetSubWidget(OcWidgetHandle widget)
{
    //m_slot.get()->SetSlotWidget(widget);
}
