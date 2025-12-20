#include "Engine/Oracle/Public/Widgets/OcWidget.h"

void OcWidget::LayoutChildren(const OracleLayout& Layout, OracleLayoutedWidgets& LayoutedWidgets)
{
    OnLayoutChildren(Layout, LayoutedWidgets);
}

void OcWidget::ComputeDesiredSize(float ScaleMultiplier)
{
    OnPreComputeDesiredSize(ScaleMultiplier);
    OnComputeDesiredSize(ScaleMultiplier);
    OnPostComputeDesiredSize(ScaleMultiplier);
}

void OcWidget::SetDesiredSize(const Vector2& DesireSize)
{
    m_desired_size = DesireSize;
}

Vector2 OcWidget::GetDesiredSize()
{
    return m_desired_size;
}

void OcWidget::OnPreComputeDesiredSize(float ScaleMultiplier)
{
}
void OcWidget::OnPostComputeDesiredSize(float ScaleMultiplier)
{
}

void OcWidget::PullEvent()
{

}

void OcWidget::Paint(OcacleDrawElementList& DrawElements, const OracleLayout& Layout)
{
    OnPaint(DrawElements, Layout);
}