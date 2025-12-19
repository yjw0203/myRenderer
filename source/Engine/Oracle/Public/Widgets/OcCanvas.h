#pragma once
#include "Engine/Oracle/Public/Widgets/OcPanel.h"

class OcCanvas : public OcPanel
{
public:
    virtual void OnComputeDesiredSize(float ScaleMultiplier) override;
    virtual void OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets) override;
    virtual void OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout) override;
};
