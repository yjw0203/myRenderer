#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleBoxDrawElement.h"

void OracleDrawElement::SetLayout(const OracleLayout* Layout, const OracleGlobalDrawInfo& draw_info)
{
    // layout to render transform
    m_transform.local_position = Layout->m_absolute_position / draw_info.m_resolution;
    m_transform.transform = Matrix2x2(Layout->m_size.x / draw_info.m_resolution.x, 0, 0, Layout->m_size.y / draw_info.m_resolution.y);
}

void OracleDrawElement::MakeBox(OcacleDrawElementList& ElementList, const OracleStyle& Style, const OracleLayout& Layout)
{
    OracleBoxDrawElement& Element = ElementList.AddUninitialized<OracleBoxDrawElement>();
    Element.SetStyle(&Style);
    Element.SetLayout(&Layout, ElementList.GetDrawInfo());
}