#include "Engine/Oracle/Public/Widgets/OcSlotWidget.h"

OcSlotWidget::OcSlotWidget()
{
    m_slot = OcMakeShared<OracleWidgetSlot>(this);
}

void OcSlotWidget::OnPreComputeDesiredSize(float ScaleMultiplier)
{
    if (m_slot.get()->IsBounding())
    {
        m_slot.get()->GetBounding()->ComputeDesiredSize(ScaleMultiplier);
    }
}

void OcSlotWidget::SetSubWidget(OcWidgetHandle widget)
{
    m_slot.get()->SetSlotWidget(widget);
}
