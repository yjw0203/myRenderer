#pragma once
#include "Engine/Oracle/Public/Widgets/OcWidget.h"
#include "Engine/Oracle/Public/Layout/OracleWidgetSlot.h"

class OcPanel : public OcWidget
{
public:
    virtual void OnPreComputeDesiredSize(float ScaleMultiplier) override;
    OracleWidgetSlotHandle AddSubWidget(OcWidgetHandle widget);
public:
    OcArray<OracleWidgetSlotHandle> m_slots;
};
