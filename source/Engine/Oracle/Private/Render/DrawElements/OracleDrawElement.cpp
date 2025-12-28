#include "Engine/Oracle/Public/Render/DrawElements/OracleDrawElement.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleBoxDrawElement.h"
#include "Engine/Oracle/Public/Render/DrawElements/OracleTextDrawElement.h"

Vector2 OracleRenderTransform::GetScale()
{
    // todo : consider rotation
    return Vector2(transform[0][0], transform[1][1]);
}

Vector2 OracleRenderTransform::GetTranslate()
{
    return local_position;
}

void OracleDrawElement::SetLayout(const OracleLayout* Layout, const OracleGlobalDrawInfo& draw_info)
{
    // layout to render transform
    m_transform.local_position = Layout->m_absolute_position / draw_info.m_resolution;
    m_transform.transform = Matrix2x2(Layout->m_size.x / draw_info.m_resolution.x, 0, 0, Layout->m_size.y / draw_info.m_resolution.y);
    m_resolution = draw_info.m_resolution;
}

void OracleDrawElement::MakeBox(OcacleDrawElementList& ElementList, const OracleStyle& Style, const OracleLayout& Layout)
{
    OracleBoxDrawElement& Element = ElementList.AddUninitialized<OracleBoxDrawElement>();
    Element.SetStyle(&Style);
    Element.SetLayout(&Layout, ElementList.GetDrawInfo());
}

void OracleDrawElement::MakeText(OcacleDrawElementList& ElementList, const std::string& text, const OracleFontStyle& Style, const OracleLayout& Layout)
{
    OracleTextDrawElement& Element = ElementList.AddUninitialized<OracleTextDrawElement>();
    Element.SetStyle(&Style);
    Element.SetLayout(&Layout, ElementList.GetDrawInfo());
    Element.SetText(text);
}