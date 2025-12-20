#pragma once
#include "Engine/Oracle/Public/Utils/Utils.h"
#include "Engine/Oracle/Public/Layout/OracleLayout.h"
#include "Engine/Oracle/Public/Layout/OracleWidgetSlot.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"

class OcWidget
{
public:
    virtual void LayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets);
    virtual void ComputeDesiredSize(float ScaleMultiplier);
    virtual void PullEvent();
    virtual void Paint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout);

    virtual void OnPreComputeDesiredSize(float ScaleMultiplier);
    virtual void OnComputeDesiredSize(float ScaleMultiplier) = 0;
    virtual void OnPostComputeDesiredSize(float ScaleMultiplier);

    virtual void OnLayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets) = 0;
    virtual void OnPaint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout) = 0;

    void SetDesiredSize(const Vector2& DesireSize);
    Vector2 GetDesiredSize();

private:
    Vector2 m_desired_size{};

public:
    OcStyleHandle m_style{};
};

using OcWidgetHandle = OcPtr<OcWidget>;

#define OcNew(WidgetType) WidgetType::Argument{OcMakeShared<WidgetType>()}