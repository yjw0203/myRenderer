#pragma once
#include "Engine/Oracle/Public/Widgets/OcWidget.h"
#include "Engine/Oracle/Public/Layout/OracleWidgetSlot.h"

class OcSlotWidget : public OcWidget
{
public:
    OcSlotWidget();
    virtual void OnPreComputeDesiredSize(float ScaleMultiplier) override;
    void SetSubWidget(OcWidgetHandle widget);
private:
    OracleWidgetSlotHandle m_slot{};
};
