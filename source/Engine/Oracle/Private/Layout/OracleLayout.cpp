#include "Engine/Oracle/Public/Layout/OracleLayout.h"

void OracleLayoutedWidgets::AddWidget(const OracleLayout& layout, OcWidgetHandle widget)
{
    m_layouted_widgets.push_back(OcPair<OracleLayout, OcWidgetHandle>(layout, widget));
}

OcArray<OcPair<OracleLayout, OcWidgetHandle>>& OracleLayoutedWidgets::GetArray()
{
    return m_layouted_widgets;
}